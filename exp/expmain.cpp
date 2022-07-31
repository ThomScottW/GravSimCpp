#include <iostream>
#include <vector>
#include "MotionVector.hpp"



int main()
{
    std::cout << "Experimentation main function successfully running!" << std::endl;

    std::vector<int> balls = {1, 2, 3, 4, 5, 6};


    for (auto it = balls.begin(); it != balls.end();)
    {
        if ((*it) % 2 == 0)
        {
            it = balls.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (int i : balls)
    {
        std::cout << i << std::endl;
    }


    return 0;
}
