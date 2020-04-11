#include "../src/reference_counter.cpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>



// class A {
//     private:
//         int x = -1;

//     public:
//         A();
//         A(int const x);
//         A(A const & other);
//         ~A();

//         void operator=(A const & other);

//         void setX(int const x);
//         int const getX() const;
// };
// A::A() { printf("default\n"); return; }
// A::A(int const x) : x(x) { printf("regular\n"); return; }
// A::A(A const & other) : x(other.getX()) { printf("copy\n"); return; }
// A::~A() { printf("destructor\n"); return; }

// void A::operator=(A const & other) {
//     this->x = other.getX();
//     printf("assignment operator\n");
//     return;
// }

// void A::setX(int const x) { this->x = x; return; }
// int const A::getX() const { return this->x; }


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

class A {
    private:
        int x = -1;

    public:
        A();
        A(int const x);
        A(A const &);
        virtual ~A();

        void operator=(A const & other);

        void setX(int const x);
        int const getX() const;
        void printMe() const;
};
class B : public A {
    public:
        B();
        B(B const &);
        ~B();
};

A::A() { printf("A_dflt\n"); return; }
A::A(int const x) : x(x) { printf("A_rglr\n"); return; }
A::A(A const & other) : x(other.getX()) { printf("A_cpy\n"); return; }
A::~A() { printf("A_dstr\n"); return; }

void A::operator=(A const & other) { printf("A_asn_opr\n"); this->x = other.getX(); }

void A::setX(int const x) { this->x = x; return; }
int const A::getX() const { return this->x; }
void A::printMe() const { printf("%i\n",this->x); return; }



B::B() : A() { printf("B_dflt\n"); return; }
B::B(B const & other) : A() { printf("B_cpy\n"); return; }
B::~B() { printf("B_dstr\n"); return; }

int main() {
    limit_ptr<A> a1(new A(1));
    limit_ptr<A> a2(new A(2));

    a1->printMe();
    a2->printMe();

    a2 = a1;
    printf("\n\n");

    a1->printMe();
    a2->printMe();

    a1 = a2;
    printf("\n\n");

    a1->printMe();
    a2->printMe();

    printf("exiting main\n");

    return 0;
}  