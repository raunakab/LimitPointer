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
Each limit_ptr wrapper object is just 2 pointers: one to the actual subject in question, and another to a local std::pair<unsigned int,unsigned int const>