#include "Attacker.hpp"


Attacker::Attacker(double x, double y)
    :Particle{
        5,
        x, y,
        MotionVector<double>(0, 0),
        SDL_Color{255, 0, 0}
    }, weaponStrength{1}
{
}


void Attacker::move()
{
    if (fixed)
    {
        return;
    }

    // Choose a random angle.
    double angle = std::fmod(std::rand(), 2 * 3.14);

    int magnitude = 50;

    double dx = magnitude * std::cos(angle);
    double dy = magnitude * std::sin(angle);

    x_pos += dx * (1 / 60.0);
    y_pos += dy * (1 / 60.0);
}


void Attacker::selectTarget(std::list<Particle> particles)
{
    return;
}


void Attacker::fire()
{
    return;
}


void Attacker::increaseWeaponStrength(int amount)
{
    weaponStrength += 1;
}
