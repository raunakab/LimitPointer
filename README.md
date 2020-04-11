# Introduction
This is a light-weight mechanism for tracking pointers. Allows one to write "Pythonic" code: code without really having to think of the underlying memory implementation.

# Features
Allows for references to objects to be tracked; frees memory when no more references to the object exist.
Basic pointer manipulation, similar to how regular pointer manipulation, can be performed.
This is because the operator*, operator=, and operator-> are all overloaded to make usage of the limit_ptr wrapper object as simple as possible.

With regular pointers, the following would result in memory leakages and dangling pointers.

    #include "/path/to/ReferenceCounter/src/reference_counter.cpp"

    class A { /* ... */ };
    int main() {
        A * a1(new A());
        A * a2(new A());

        a2 = a1;            // memory leak
        
        delete a2;
        a2 = nullptr;

        a1->doSomething();  // dangling pointer

        return 0;
    }

But with limit_ptrs, the above code can be typed out without having to think about the above fatal errors.

    #include "/path/to/ReferenceCounter/src/reference_counter.cpp"

    class A { /* ... */ };
    int main() {
        limit_ptr<A> a1(new A());
        limit_ptr<A> a2(new A());

        a2 = a1;                    // no memory leak; since there are no more references to *a2, a2 is deleted before switching over
        
        // delete a2;               // no explicit deletes are allowed; deletes are only by the limit_ptr mechanism
        a2 = nullptr;

        a1->doSomething();          // not a dangling pointer

        a1 = nullptr;               // deletes *a1 since no more references point to it; A::~A() is invoked

        return 0;                   // code exits with neither a segmentation fault or memory leak
    }


# Implementation
Each limit_ptr wrapper object is just 2 pointers: one to the actual subject in question, and another to a std::pair<unsigned int,unsigned int const>.
Let the pointer to the actual object be called __*subject*__ and the pointer to the std::pair be called __*properties*__.
The __*properties*__ pointer a pair that contains 2 unsigned integers: the first corresponding to the reference count, the other to a limit.

The reference count denotes how many pointers point to this object. The count should always be between 1 <= count <= limit.
The reference count is incremented when a new limit_ptr starts pointing this object and decremented when an existing one switches to another subject.
As soon as the reference count hits 0, the __*subject*__ is deleted, along with __*properties*__.

The limit denotes the maximum number of items that are allowed to point to the subject.
This unsigned integer is a constant and cannot be changed after instantiation!
If another object tries to switch to pointing to an object at maximum capacity, it will be denied access and remain pointing to its original target.
However, if limit is 0, then no limits to the number of references are imposed.

# Rules
In order for a limit_ptr to be considered a valid object, two rules need to be followed:
    1)  __*subject*__ AND __*properties*__ are either BOTH non-null or are BOTH null.
        If, somehow, a limit_ptr exists with only 1 of the above being null, this is considered a MALFORMED object.
    2)  If __*subject*__ and __*properties*__ are BOTH non-null, and if __*properties->second*__ (a.k.a., the limit) is not 0, then __*properties->first*__ (a.k.a., the count) has to be between 1 and limit.
        If, somehow, the count is not within 1 and limit, then this limit_ptr will be considered a MALFORMED object.
If the above two rules are satisfied, then this limit_ptr wrapper object will be considered a WELLFORMED object.

Multiple precautions have been taken in order to ensure that, in the case a limit_ptr object is compromised and converted into a MALFORMED object, it does not spread its malformity to others.
For example, consider the following:

    #include "/path/to/ReferenceCounter/src/reference_counter.cpp"

    class A { /* ... */ };
    int main() {
        limit_ptr<A> a1(new A());
        limit_ptr<A> a2(new A());

        ...
        /* a1 somehow becomes a MALFORMED object here */
        ...

        a2 = a1;            // ** Note 1 **
        a1 = a2;            // ** Note 2 **
    }

__*Note 1:*__
Here, the assignment operator would return false, meaning that the assignment did NOT work and a2 still contains its original content.
This prevents the malformity to spreading to the WELLFORMED a2 object.

__*Note 2:*__
Here, the assignment operator would fail again. This may seem unintuitive; why prevent a malformed object from being turned into a wellformed object again?
Well, suppose two limit_ptrs pointed to two separate *__subject__* objects, but their *__properties__* but pointed towards the same std::pair.
Therefore, if the first object switched away, it would decremented the count. But since the limit_ptrs both point to the same *__properties__* object, an improper decrementation would occur.
Thus, a malformed limit_ptr will always remain a malformed limit_ptr.

When developing this class, much attention was paid to ensuring that malformed objects do not ever arise. (Thus, if malformed objects ever arise, something has gone very, very wrong...)
I also paid much attention to ensuring that MALFORMED objects do not proliferate and spread their malformity to other objects.
This is why operations, such as assignment, to or from a alformed object, do not occur!