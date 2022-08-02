#include <iostream>
#include <vector>
#include "MotionVector.hpp"



class A
{
public:
    A()=default;

    virtual ~A()=default;

    virtual void move() {std::cout << "A::move()" << this << std::endl;}
};




class B : public A
{
public:
    B() {};
    virtual ~B()=default;

    void move() override {std::cout << "B::move()" << std::endl;}
};



int main()
{
    std::cout << fmod(-3, 6) << std::endl;
    
}
