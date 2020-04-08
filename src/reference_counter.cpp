#include <stdlib.h>
#include <stdio.h>
#include <iostream>



template<class T> class limit_ptr {
    private:
        T * subject = nullptr;
        std::pair<int,int const> * count = nullptr;

        T * const get_subject_ptr() const;
        std::pair<int,int const> * const get_count_ptr() const;

        bool const switch_away();
        bool const switch_to(T * const subject, std::pair<int,int const> & count);

    public:
        limit_ptr();
        limit_ptr(T * const && subject);
        limit_ptr(T * const && subject, int const limit);
        limit_ptr(limit_ptr<T> const & other);
        ~limit_ptr();

        void operator=(limit_ptr<T> const & other);
        bool const operator==(limit_ptr<T> const & other) const;
        bool const operator!=(limit_ptr<T> const & other) const;
        T * const operator->() const;
        T & operator*() const;

        void switchFocus(T * const && subject, int const limit);
        void setNullptr();

        int const getCount() const;
        int const getLimit() const;

        bool const atCapacity(std::pair<int,int const> & count) const;
        bool const isWellformed() const;
        bool const isWellformedNonNull() const;
        bool const isWellformedNull() const;

        void deepCopy(int const limit);
        void deepCopy(limit_ptr<T> const & other, int const limit);
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
template<class T> std::pair<int,int const> * const limit_ptr<T>::get_count_ptr() const { return this->count; }



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
    if (!this->isWellformedNonNull()) return false;

    int const count(--(this->count->first));

    if (!count) {
        delete this->count;
        delete this->subject;
    }

    this->count = nullptr;
    this->subject = nullptr;
    return true;
}
/*
 *  @param_1: std::pair<int,int const> &
 *      A reference to an int and a constant int.
 *      This is the COUNT associated with the new SUBJECT, @param_2.
 *  @param_2: T * const:
 *      A constant pointer to a T-templated object.
 *      This is the new SUBJECT to focus on.
 *
 *  @implementation:
 *      A constant rvalue boolean is returned; false if COUNT or SUBJECT are not nulled out before invoking this function, true if otherwise.
 *      The function evaluates to see if the limit has been reached; if so, it will create a NEW COPY of the existing COUNT and SUBJECT (with COUNT's count to be 0).
 *      Else, @param_1 will be set as the new COUNT, the normal incrementation will be performed, and @param_2 will become the new SUBJECT.
 *
 *  @purpose:
 *      @TODO
*/
template<class T> bool const limit_ptr<T>::switch_to(T * const subject, std::pair<int,int const> & count) {
    if (!this->isWellformedNull()) return false;
    else if (!subject) return true;
    else if (this->atCapacity(count)) this->switchFocus(new T(*subject),count.second);
    else {
        this->count = &count;
        ++(this->count->first);
        this->subject = subject;
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
template<class T> limit_ptr<T>::limit_ptr(T * const && subject) : subject(subject), count(subject ? (new std::pair<int,int const>(1,-1)) : nullptr) { return; }
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
template<class T> limit_ptr<T>::limit_ptr(T * const && subject, int const limit) : subject(subject), count(subject ? (new std::pair<int,int const>(1,(limit >= 1) ? limit : -1)) : nullptr) { return; }
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
template<class T> limit_ptr<T>::limit_ptr(limit_ptr<T> const & other) { this->switch_to(other.get_subject_ptr(),*other.get_count_ptr()); return; }
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
template<class T> limit_ptr<T>::~limit_ptr() { this->switch_away(); return; }



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
template<class T> void limit_ptr<T>::operator=(limit_ptr<T> const & other) {
    this->switch_away();
    this->switch_to(other.get_subject_ptr(),*other.get_count_ptr());

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
template<class T> bool const limit_ptr<T>::operator==(limit_ptr<T> const & other) const { return (this->subject == other.get_subject_ptr()); }
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
template<class T> T * const limit_ptr<T>::operator->() const { return this->subject; }
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
template<class T> T & limit_ptr<T>::operator*() const { return *(this->subject); }



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
template<class T> void limit_ptr<T>::switchFocus(T * const && subject, int const limit) {
    if (!this->isWellformed()) return;

    this->switch_away();
    if (!subject) return;

    std::pair<int,int const> * const temp_count = new std::pair<int,int const>(0,(limit >= 1) ? limit : -1);
    T * const temp_subject = subject;
    this->switch_to(temp_subject,*temp_count);

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
template<class T> void limit_ptr<T>::setNullptr() {
    this->switchFocus(nullptr,this->count->second);
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
template<class T> int const limit_ptr<T>::getCount() const { return this->count->first; }
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
template<class T> int const limit_ptr<T>::getLimit() const { return this->count->second; }



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
template<class T> bool const limit_ptr<T>::atCapacity(std::pair<int,int const> & count) const {
    if (count.second < 1) return false;
    return ((count.first) >= (count.second));
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
    if (this->subject && this->count) {
        int const count(this->count->first);
        int const limit(this->count->second);

        return (limit <= 0) || ((1 <= count) && (count <= limit));
    } else return false;
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
template<class T> bool const limit_ptr<T>::isWellformedNull() const { return !this->subject && !this->count; }



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
template<class T> void limit_ptr<T>::deepCopy(int const limit) {
    if (!this->isWellformedNonNull()) return;

    this->switchFocus(new T(*this->subject),(limit >= 1) ? limit : -1);
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
template<class T> void limit_ptr<T>::deepCopy(limit_ptr<T> const & other, int const limit) {
    if (!this->isWellformed() || !other.isWellformed()) return;
    else if (!other.isWellformedNonNull()) {
        this->switch_away();
        return;
    }

    T * const otherSubject = other.get_subject_ptr();
    this->switchFocus(new T(*otherSubject),(limit >= 1) ? limit : -1);
    return;
}