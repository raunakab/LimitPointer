#include "../src/reference_counter.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>



class A {
    private:
        int x = -1;

    public:
        A();
        A(int const x);
        A(A const & other);
        ~A();

        void operator=(A const & other);

        void setX(int const x);
        int const getX() const;
};
A::A() { printf("default\n"); return; }
A::A(int const x) : x(x) { printf("regular\n"); return; }
A::A(A const & other) : x(other.getX()) { printf("copy\n"); return; }
A::~A() { printf("destructor\n"); return; }

void A::operator=(A const & other) {
    this->x = other.getX();
    printf("assignment operator\n");
    return;
}

void A::setX(int const x) { this->x = x; return; }
int const A::getX() const { return this->x; }


// int main() {
//     limit_ptr<A> a(new A(1));
//     limit_ptr<A> b(new A(2));
    
//     int a_x;
//     int b_x;

//     a_x = a->getX();
//     b_x = b->getX();
//     printf("a_x:\t%i\n",a_x);
//     printf("b_x:\t%i\n",b_x);

//     b = a;

//     a_x = a->getX();
//     b_x = b->getX();
//     printf("a_x:\t%i\n",a_x);
//     printf("b_x:\t%i\n",b_x);

//     b.deepCopy();
//     b->setX(22);

//     a_x = a->getX();
//     b_x = b->getX();
//     printf("a_x:\t%i\n",a_x);
//     printf("b_x:\t%i\n",b_x);

//     printf("About to exit main.\n");

//     return 0;
// }

int main() {
    limit_ptr<int> a(new int(5));
}