#include <iostream>
#include <vector>
#include "MotionVector.hpp"



class A
{
public:
    A()=default;

    ~A()=default;

    virtual void move() {std::cout << "A::move()" << std::endl;}
};




class B : public A
{
public:
    B() {};
    ~B()=default;

    void move() override {std::cout << "B::move()" << std::endl;}
};



int main()
{
    std::cout << "Experimentation main function successfully running!" << std::endl;


    std::vector<A*> vec = {};


    A a = A();

    vec.push_back(&a);
    
    
}
