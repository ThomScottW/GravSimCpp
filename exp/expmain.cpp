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
    std::cout << "Experimentation main function successfully running!" << std::endl;


    std::vector<A*> vec = {};



    A* ap = new A();

    vec.push_back(ap);
    
    while (!vec.empty())
    {
        delete vec.back();
        vec.pop_back();
    }


    A* bp = new A();

    A* cp = bp;

    bp->move();

    std::cout << bp << std::endl;
    std::cout << cp << std::endl;
    
}
