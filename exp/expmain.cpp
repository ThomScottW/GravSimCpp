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
    int low = 1;
    int high = 9;

    for (int i = 0; i < 10; ++i)
    {
        std::cout << std::rand() % high + low << std::endl;
    }


    
}
