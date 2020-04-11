#include <stdlib.h>
#include <stdio.h>
#include <iostream>



template<class T> class limit_ptr {
    private:
        T * subject = nullptr;
        std::pair<unsigned int, unsigned int const> * properties = nullptr;

        T * const get_subject_ptr() const;
        std::pair<unsigned int,unsigned int const> * const get_properties_ptr() const;
        int const get_count() const;
        int const get_limit() const;

        bool const switch_away();
        bool const switch_away_to(T *, std::pair<unsigned int,unsigned int const> *);
        void unsafe_hard_reset();

    public:
        limit_ptr();
        limit_ptr(T * const &&);
        limit_ptr(T * const &&, unsigned int const);
        limit_ptr(limit_ptr<T> const &);
        ~limit_ptr();

        bool const operator=(limit_ptr<T> const &);
        bool const operator=(T * const &&);
        bool const operator==(limit_ptr<T> const &) const;
        bool const operator!=(limit_ptr<T> const &) const;
        T * const operator->() const;
        T & operator*() const;

        bool const switchFocus(T * const &&, unsigned int const);
        bool const setNullptr();

        unsigned int const getCount() const;
        unsigned int const getLimit() const;

        bool const atCapacity() const;
        bool const isWellformed() const;
        bool const isWellformedNonNull() const;
        bool const isWellformedNull() const;

        bool const deepCopy(unsigned int const);
        bool const deepCopy(limit_ptr<T> const &, unsigned int const);
};



/*
 *  @param: void
 *  @implementation:
 *      Returns a constant rvalue pointer to a non-const T-type object.
 *      The returned value is a pointer to this object's SUBJECT.
 *
 *  @requirement:
 *      Void.
 *  @guarantee:
 *      This function will not modify this object.
 *
 *  @purpose:
 *      To obtain a pointer to this object's SUBJECT.
 *  @usage:
 *      Method is private; reserved for internal usage only.
 *      Useful during object construction and object copy assignment to obtain a pointer to another LIMIT_PTR's SUBJECT.
*/
template<class T> T * const limit_ptr<T>::get_subject_ptr() const { return this->subject; }
/*
 *  @param: void
 *  @implementation:
 *      Returns a constant rvalue pointer to a non-const std::pair<int,int const>.
 *      The returned value is a pointer to this object's COUNT.
 *
 *  @requirement:
 *      Void.
 *  @guarantee:
 *      This function will not modify this object.
 *
 *  @purpose:
 *      To obtain a pointer to this object's COUNT.
 *  @usage:
 *      Method is private; reserved for internal usage only.
 *      Useful during object construction and object copy assignment to obtain a pointer to another LIMIT_PTR's COUNT.
*/
template<class T> std::pair<unsigned int,unsigned int const> * const limit_ptr<T>::get_properties_ptr() const { return this->properties; }
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> int const limit_ptr<T>::get_count() const { return this->properties->first; }
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> int const limit_ptr<T>::get_limit() const { return this->properties->second; }



/*
 *  @param: void
 *
 *  @implementation:
 *      A constant rvalue boolean; false if COUNT is nulled out or COUNT's count is less than 1 before invoking this function; true if otherwise.
 *      The function decrements COUNT's count; if the newly decremented value is 0, then COUNT and SUBJECT are deleted.
 *      COUNT and SUBJECT are then, regardless, set to nullptrs.
 *
 *  @guarantee:
 *      COUNT's count will never be decremented to below 1 (except, it can be decremented to 0, but then COUNT and SUBJECT are deleted).
 *
 *  @purpose:
 *      As the function's name suggests, to "switch-away" focus from one object pointer.
 *      Therefore, the COUNT first needs to be decremented (and COUNT and SUBJECT need to be deleted if COUNT's count is 0), and then COUNT and SUBJECT need to be nulled.
 *      This is defined as the action of "switching away focus" from a particular object, since this wrapper is no longer paying attention to it.
 *      Since this function is only responsible for switching focus away from a particular source, it nulls COUNT and SUBJECT.
 *      This function is not responsible for switching focus to a new object!
 *      This function cannot switch away its focus if it has already switched away its focus before!
*/
template<class T> bool const limit_ptr<T>::switch_away() {
    if (!this->isWellformedNonNull()) return this->isWellformedNull();

    unsigned int const count(--(this->properties->first));
    if (!count) {
        delete this->subject;
        delete this->properties;
    }

    this->subject = nullptr;
    this->properties = nullptr;

    return true;
}
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::switch_away_to(T * subject, std::pair<unsigned int, unsigned int const> * properties) {
    if ((!subject != !properties) || ((this->subject == subject) != (this->properties == properties))) return false;
    else if ((this->subject == subject) && (this->properties == properties)) return this->isWellformed();
    else if (subject && properties) {
        unsigned int const count(properties->first);
        unsigned int const limit(properties->second);

        if (limit && (limit <= count)) return false;
    }
    
    if (!this->switch_away()) return false;
    else if (!subject && !properties) return true;

    this->subject = subject;
    this->properties = properties;
    ++(this->properties->first);

    return true;
}



/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> limit_ptr<T>::limit_ptr() { return; }
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> limit_ptr<T>::limit_ptr(T * const && subject) : subject(subject), properties(subject ? (new std::pair<unsigned int,unsigned int const>(1,0)) : nullptr) { return; }
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> limit_ptr<T>::limit_ptr(T * const && subject, unsigned int const limit) : subject(subject), properties(subject ? (new std::pair<unsigned int,unsigned int const>(1,limit)) : nullptr) { return; }
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> limit_ptr<T>::limit_ptr(limit_ptr<T> const & other) {
    if (other.isWellformedNonNull()) this->switch_away_to(other.get_subject_ptr(),other.get_properties_ptr());
    return;
}
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> limit_ptr<T>::~limit_ptr() {
    this->switch_away();
    return;
}



/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::operator=(limit_ptr<T> const & other) {
    if (other.isWellformed()) return this->switch_away_to(other.get_subject_ptr(),other.get_properties_ptr());
    return false;
}
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::operator=(T * const && subject) {
    std::pair<unsigned int,unsigned int const> * const temp_properties(subject ? (new std::pair<unsigned int,unsigned int const>(0,0)) : nullptr);
    if (!this->switch_away_to(subject,temp_properties)) {
        if (subject) delete subject;
        if (temp_properties) delete temp_properties;

        return false;
    }

    return true;
}
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::operator==(limit_ptr<T> const & other) const {
    if (!this->isWellformed() || !other.isWellformed()) return false;
    return (this->subject == other.get_subject_ptr()) && (this->properties == other.get_properties_ptr());
}
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::operator!=(limit_ptr<T> const & other) const { return !this->operator==(other); }
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> T * const limit_ptr<T>::operator->() const { return this->isWellformedNonNull() ? this->subject : nullptr; }
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> T & limit_ptr<T>::operator*() const { return this->isWellformedNonNull() ? *this->subject : *(new limit_ptr<T>()); }



/*
 *  @param_1: T * const &&
 *      An rvalue reference to a constant pointer to a T-type object.
 *      This is a pointer to the object which will act as the source for the copy.
 *  @param_2: int const
 *      A constant integer.
 *      This integer serves as the new limit for the to-be-copied SUBJECT.
 *  @implementation:
 *      Returns void.
 *      The function aims to make a new, deep copy of @param_1.
 *      First, focus is switched away from the current SUBJECT.
 *      If this object is malformed, then the function will return without doing anything.
 *      If this object is well-formed, then COUNT and SUBJECT will be set to nullptrs, regardless of them either being both nullptrs or non-nullptrs to begin with.
 *      If @param_1 is a nullptr, then the function will return (but note that both SUBJECT and COUNT will have been set to nullptrs by this time).
 *      If @param_1 is not a nullptr, then focus will be switched to this new SUBJECT.
 *      Seat count will be incremented to 1, and the limit will be set to @param_2.
 *      A new SUBJECT will be constructed, with @param_1 as the source of the copy.
 *          SUBJECT?   |   COUNT?   |   @param_1?   |   Result
 *          0          |   0        |   0           |   SUBJECT and COUNT are already both nullptrs; return without doing anything.
 *          0          |   0        |   1           |   A new SUBJECT and COUNT are constructed and focus is switched to it; seat_count is set to 1 and limit set to @param_2.
 *          0          |   1        |   0           |   Malformed object; return without doing anything.
 *          0          |   1        |   1           |   Malformed object; return without doing anything.
 *          1          |   0        |   0           |   Malformed object; return without doing anything.
 *          1          |   0        |   1           |   Malformed object; return without doing anything.
 *          1          |   1        |   0           |   SUBJECT and COUNT will be both set to nullptrs.
 *          1          |   1        |   1           |   Focus is switched away from SUBJECT. A new SUBJECT and COUNT are constructed and focus is switched to it; seat_count is set to 1 and limit set to @param_2.
 *
 *  @requirement:
 *      Void.
 *      (@param_1 can either be a non-nullptr or a nullptr; @param_2 can be any integer).
 *  @guarantee:
 *      This function will preserve the validity of this object.
 *      i.e. If this object is malformed, this method will not touch nor modify it; if this object is well-formed, then it will emerge changed, but still well-formed.
 *
 *  @purpose:
 *      To switch focus away from the current SUBJECT, construct a new, full, deep copy of the passed in pointer if @param_1 is a non-nullptr, and switch focus to it.
 *  @usage:
 *      Method is private; reserved for internal usage only.
 *      Useful to easily and safely switch focus from an old subject to a new subject.
*/
template<class T> bool const limit_ptr<T>::switchFocus(T * const && subject, unsigned int const limit) {
    std::pair<unsigned int,unsigned int const> * const temp_properties(subject ? (new std::pair<unsigned int,unsigned int const>(0,limit)) : nullptr);
    if (!this->switch_away_to(subject,temp_properties)) {
        if (subject) delete subject;
        if (temp_properties) delete temp_properties;
        
        return false;
    }

    return true;
}
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::setNullptr() { return this->switch_away(); }



/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> unsigned int const limit_ptr<T>::getCount() const { return this->isWellformedNonNull() ? this->properties->first : 0; }
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> unsigned int const limit_ptr<T>::getLimit() const { return this->isWellformedNonNull() ? this->properties->second : 0; }



/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::atCapacity() const {
    if (!this->isWellformedNonNull()) return false;

    unsigned int limit(this->get_limit());
    unsigned int count(this->get_count());

    return limit ? (count >= limit) : false;
}
/*
 *  @param: void
 *  @implementation:
 *      Returns a constant rvalue boolean; true if either both COUNT and SUBJECT are nullptr, or if both COUNT and SUBJECT are non-nullptrs, false otherwise.
 *
 *  @requirement:
 *      Void.
 *  @guarantee:
 *      This function will not modify this object.
 *
 *  @purpose:
 *      A LIMIT_PTR object is considered "broken" or "malformed" if only one of its members (COUNT or SUBJECT) are nullptrs.
 *      In order to be considered a well-formed object, either both members need to be non-nullptrs, or both nullptrs.
 *      (This is because a counter should not be placed and maintained/incremented/decremented for a nullptr).
 *      This functions evaluates this object and determines its validity based on these class specifications.
 *  @usage:
 *      Method is private; reserved for internal usage only.
 *      Useful to quickly determine the validity of this object.
*/
template<class T> bool const limit_ptr<T>::isWellformed() const { return this->isWellformedNonNull() || this->isWellformedNull(); }
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::isWellformedNonNull() const {
    if (this->subject && this->properties) {
        unsigned int const count(this->properties->first);
        unsigned int const limit(this->properties->second);

        return (!limit) || ((1 <= count) && (count <= limit));
    }
    return false;
}
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::isWellformedNull() const { return !this->subject && !this->properties; }



/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::deepCopy(unsigned int const limit) {
    if (!this->isWellformedNonNull()) return this->isWellformedNull();

    T * const temp_subject(new T(*this->subject));
    std::pair<unsigned int,unsigned int const> * const temp_properties(new std::pair<unsigned int,unsigned int const>(0,limit));

    if (!this->switch_away_to(temp_subject,temp_properties)) {
        delete temp_subject;
        delete temp_properties;

        return false;
    }
    
    return true;
}
/*
 *  @param:
 *  @implementation:
 *
 *  @requirements:
 *  @guarantee:
 *
 *  @purpose:
 *  @usage:
*/
template<class T> bool const limit_ptr<T>::deepCopy(limit_ptr<T> const & other, unsigned int const limit) {
    T * temp_subject(nullptr);
    std::pair<unsigned int,unsigned int const> * temp_properties(nullptr);

    if (other.isWellformedNonNull()) {
        temp_subject = new T(*other.get_subject_ptr());
        temp_properties = new std::pair<unsigned int,unsigned int const>(0,limit);
    } else if (!other.isWellformedNull()) return false;

    if (!this->switch_away_to(temp_subject,temp_properties)) {
        if (temp_subject) delete temp_subject;
        if (temp_properties) delete temp_properties;

        return false;
    }

    return true;
}