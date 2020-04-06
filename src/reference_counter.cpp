#include <stdlib.h>
#include <stdio.h>
#include <iostream>



template<class T> class limit_ptr {
    private:
        T * subject = nullptr;
        std::pair<int,int const> * count = nullptr;

        bool const is_wellformed() const;

        T * const get_subject_ptr() const;
        std::pair<int,int const> * const get_count_ptr() const;
        void new_copy(T * const subject, int const limit);

        bool const switch_away();
        bool const switch_to(T * const subject, std::pair<int,int const> & count);
        bool const switch_to(limit_ptr<T> const & other);
        bool const at_capacity(std::pair<int,int const> & count) const;

    public:
        limit_ptr();
        limit_ptr(T && subject);
        limit_ptr(T && subject, int const limit);
        limit_ptr(limit_ptr<T> const & other);
        ~limit_ptr();

        void operator=(limit_ptr<T> const & other);
        bool const operator==(limit_ptr<T> const & other) const;
        bool const operator!=(limit_ptr<T> const & other) const;
        T * const operator->() const;
        T & operator*() const;

        void set(T && subject, int const limit);

        int const getCount() const;
        int const getLimit() const;

        void deepCopy();
        void deepCopy(int const limit);
        void deepCopy(limit_ptr<T> const & other);
        void deepCopy(limit_ptr<T> const & other, int const limit);
};



template<class T> bool const limit_ptr<T>::is_wellformed() const { return (this->subject && this->count) || (!this->subject && !this->count); }

template<class T> T * const limit_ptr<T>::get_subject_ptr() const { return this->subject; }
template<class T> std::pair<int,int const> * const limit_ptr<T>::get_count_ptr() const { return this->count; }
template<class T> void limit_ptr<T>::new_copy(T * const subject, int const limit) {
    this->switch_away();

    if (!this->is_wellformed() || !subject) return;

    std::pair<int,int const> * const temp_count = new std::pair<int,int const>(0,(limit >= 1) ? limit : -1);
    T * const temp_subject = new T(*subject);
    this->switch_to(temp_subject,*temp_count);

    return;
}

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
    if (!this->count || !this->subject || (this->count->first < 1)) return false;

    --(this->count->first);
    if (!(this->count->first)) {
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
    if (this->count || this->subject) return false;
    else if (!subject) return true;
    else if (this->at_capacity(count)) this->new_copy(subject,count.second);
    else {
        this->count = &count;
        ++(this->count->first);
        this->subject = subject;
    }
    return true;
}
template<class T> bool const limit_ptr<T>::switch_to(limit_ptr<T> const & other) {
    return this->switch_to(other.get_subject_ptr(), *other.get_count_ptr());
}
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
template<class T> bool const limit_ptr<T>::at_capacity(std::pair<int,int const> & count) const {
    if (count.second < 1) return false;
    return ((count.first) >= (count.second));
}

template<class T> limit_ptr<T>::limit_ptr() { return; }
template<class T> limit_ptr<T>::limit_ptr(T && subject) : subject(new T(subject)), count(subject ? (new std::pair<int,int const>(1,-1)) : nullptr) { return; }
template<class T> limit_ptr<T>::limit_ptr(T && subject, int const limit) : subject(new T(subject)), count(subject ? (new std::pair<int,int const>(1,(limit >= 1) ? limit : -1)) : nullptr) { return; }
template<class T> limit_ptr<T>::limit_ptr(limit_ptr<T> const & other) { this->switch_to(other); return; }
template<class T> limit_ptr<T>::~limit_ptr() { this->switch_away(); return; }

template<class T> void limit_ptr<T>::operator=(limit_ptr<T> const & other) {
    this->switch_away();
    this->switch_to(other);

    return;
}
template<class T> bool const limit_ptr<T>::operator==(limit_ptr<T> const & other) const { return ((this->subject) == (other.get_subject_ptr())) || (*(this->subject) == *(other.get_subject_ptr())); }
template<class T> bool const limit_ptr<T>::operator!=(limit_ptr<T> const & other) const { return !this->operator==(other); }
template<class T> T * const limit_ptr<T>::operator->() const { return this->subject; }
template<class T> T & limit_ptr<T>::operator*() const { return *(this->subject); }

template<class T> void limit_ptr<T>::set(T && subject, int const limit) {
    this->new_copy(subject,(limit >= 1) ? limit : -1);
    return;
}

template<class T> int const limit_ptr<T>::getCount() const { return this->count->first; }
template<class T> int const limit_ptr<T>::getLimit() const { return this->count->second; }

template<class T> void limit_ptr<T>::deepCopy() {
    if (!this->count || !this->subject) return;

    int const x(this->count->second);
    this->new_copy(this->subject,(x >= 1) ? x : -1);
    return;
}
template<class T> void limit_ptr<T>::deepCopy(int const limit) {
    if (!this->count || !this->subject) return;

    this->new_copy(this->subject,(limit >= 1) ? limit : -1);
    return;
}
template<class T> void limit_ptr<T>::deepCopy(limit_ptr<T> const & other) {
    int const x(-1);

    if (this->count && this->subject) x = this->count->second;
    this->new_copy(other.get_subject_ptr(),(x >= 1) ? x : -1);
    return;
}
template<class T> void limit_ptr<T>::deepCopy(limit_ptr<T> const & other, int const limit) {
    this->new_copy(other.get_subject_ptr(),(limit >= 1) ? limit : -1);
    return;
}