#include "Attacker.hpp"


Attacker::Attacker(double x, double y)
    :Particle{
        5,
        x, y,
        MotionVector<double>(0, 0),
        SDL_Color{255, 0, 0},
        false  // No gravity
    }, target{nullptr}, weaponStrength{1}, angle{-1}, lifespan{100}
{
}


void Attacker::move()
{
    if (fixed)
    {
        return;
    }

    // If there is no target, then move randomly.
    if (nullptr == target)
    {
        if (angle < 0) // If the angle hasn't been initialized yet.
        {
            // Choose a random angle.
            angle = std::fmod(std::rand(), 2 * 3.14);
        }
        else
        {
            int chance = std::rand() % 100;

            if (chance < 20)
            {
                angle = std::fmod(std::rand(), 2 * 3.14);
            }
        }
    }
    
    int magnitude = 50;

    double dx = magnitude * std::cos(angle);
    double dy = magnitude * std::sin(angle);

    x_pos += dx * (1 / 60.0);
    y_pos += dy * (1 / 60.0);
}


void Attacker::selectTarget(Particle* target)
{
    target = target;
}


void Attacker::fire()
{
    return;
}


void Attacker::increaseWeaponStrength(int amount)
{
    weaponStrength += 1;
}
