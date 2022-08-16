#include "Attacker.hpp"


Attacker::Attacker(double x, double y)
    :Particle{
        5,
        x, y,
        MotionVector<double>(0, 0),
        SDL_Color{255, 0, 0},
        false  // No gravity
    }, target{nullptr}, ws{1}, angle{-1}, lifespan{100}, range{200}
{
}


void Attacker::update(std::list<Particle*>& particles)
{
    move();

    if (nullptr != target)
    {
        // Check if the target is still nearby or still exists.
        if (target->isAbsorbed() || target->getMass() <= 0 || distanceFrom(target->x(), target->y()) > range + 100)
        {
            std::cout << "Target too far away." << (target->isAbsorbed() ? 1 : 0) << std::endl;
            target = nullptr;
            ws = 1;
        }
        // If it is nearby and the attacker is locked on, inflict damage.
        else if (lockedOn())
        {
            increaseWeaponStrength(0.25);
            target->changeMass(-weaponDamage());
            if (target->getMass() <= 0)
            {
                // If the target gets destroyed, reset the weapon strength.
                std::cout << "Destroyed target." << std::endl;
                target = nullptr;
                ws = 1;
            }
            // std::cout << "Reduced target mass by " << std::pow(ws, ws / 20) << std::endl;
        }
        return;
    }

    Particle* closest = nullptr;
    double closestDist = range + 1;
    double thisDist = range + 1;

    for (Particle* p : particles)
    {
        if (( thisDist = distanceFrom(p->x(), p->y()) ) < closestDist && p != this && p->hasGravity())
        {
            closest = p;
            closestDist = thisDist;
        }
    }

    if (nullptr != closest)
    {
        target = closest;
    }
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
            int chance = std::rand() % 1000;

            if (chance < 5)
            {
                angle = std::fmod(std::rand(), 2 * 3.14);
            }
        }
    }
    // Otherwise, move towards the target.
    else
    {
        double dx = target->x() - x_pos;
        double dy = target->y() - y_pos;
        angle = std::atan2(dy, dx);
    }
    
    int magnitude = 25;

    double dx = magnitude * std::cos(angle);
    double dy = magnitude * std::sin(angle);

    if (nullptr == target || distanceFrom(target->x(), target->y()) > target->getRadius() + 100)
    {
        x_pos += dx * (1 / 60.0);
        y_pos += dy * (1 / 60.0);
    }
}


bool Attacker::lockedOn()
{
    return (nullptr != target) && (distanceFrom(target->x(), target->y()) <= range);
}


int Attacker::getWeaponStrength()
{
    return ws;
}


void Attacker::fire()
{
    return;
}


// Return a pointer to the target, if there is one.
Particle* Attacker::getTarget()
{
    return target;
}


SDL_Color Attacker::getColor()
{
    return color;
}


void Attacker::increaseWeaponStrength(double amount)
{
    ws += ws <= 200 ? amount : 0;
}


double Attacker::weaponDamage()
{
    if (ws < 20)
    {
        return 10000;
    }
    else if (ws < 40)
    {
        return 100000;
    }
    else if (ws < 60)
    {
        return 1000000;
    }
    else if (ws < 200)
    {
        return 10000000;
    }
    else 
    {
        return 100000000;
    }

}
