#include <stdlib.h>
#include <stdio.h>
#include <iostream>



template<class T> class limit_ptr {
    private:
        int * count = new int(1);
        T * subject = nullptr;

        int * const get_count_ptr() const;
        T * const get_subject_ptr() const;

        void increment_count() const;
        void decrement_count();

    public:
        limit_ptr();
        limit_ptr(T * const subject);
        limit_ptr(limit_ptr<T> const & other);
        ~limit_ptr();

        void operator=(limit_ptr<T> const & other);
        bool const operator==(limit_ptr<T> const & other) const;
        bool const operator!=(limit_ptr<T> const & other) const;
        T * const operator->() const;
        T & operator*() const;

        int const getCount() const;

        void deepCopy();
        void deepCopy(limit_ptr<T> const & other);
};

template<class T> int * const limit_ptr<T>::get_count_ptr() const { return this->count; }
template<class T> T * const limit_ptr<T>::get_subject_ptr() const { return this->subject; }

template<class T> void limit_ptr<T>::increment_count() const {
    if (this->count) ++(*(this->count));
    return;
}
template<class T> void limit_ptr<T>::decrement_count() {
    if (this->count) --(*(this->count));

    if (!(*(this->count))) {
        delete this->count;
        delete this->subject;

        this->count = nullptr;
        this->subject = nullptr;
    }

    return;
}

template<class T> limit_ptr<T>::limit_ptr() { return; }
template<class T> limit_ptr<T>::limit_ptr(T * const subject) : subject(subject) { return; }
template<class T> limit_ptr<T>::limit_ptr(limit_ptr<T> const & other) : count(other.get_count_ptr()), subject(other.get_subject_ptr()) { this->increment_count(); return; }
template<class T> limit_ptr<T>::~limit_ptr() { decrement_count(); return; }

template<class T> void limit_ptr<T>::operator=(limit_ptr<T> const & other) {
    this->decrement_count();
    this->count = other.get_count_ptr();
    this->subject = other.get_subject_ptr();
    this->increment_count();

    return;
}
template<class T> bool const limit_ptr<T>::operator==(limit_ptr<T> const & other) const { return (this->count == other.get_count_ptr()) && (this->subject == other.get_subject_ptr()); }
template<class T> bool const limit_ptr<T>::operator!=(limit_ptr<T> const & other) const { return !this->operator==(other); }
template<class T> T * const limit_ptr<T>::operator->() const { return this->subject; }
template<class T> T & limit_ptr<T>::operator*() const { return *(this->subject); }

template<class T> int const limit_ptr<T>::getCount() const { return *(this->count); }
template<class T> void limit_ptr<T>::deepCopy() {
    this->decrement_count();
    this->count = new int(1);
    this->subject = new T(*(this->subject));

    return;
}
template<class T> void limit_ptr<T>::deepCopy(limit_ptr<T> const & other) {
    this->decrement_count();
    this->count = new int(1);
    this->subject = new T(*other.get_subject_ptr());

    return;
}