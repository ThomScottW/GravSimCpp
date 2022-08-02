#ifndef ATTACKER_HPP
#define ATTACKER_HPP


#include <iostream>
#include <cmath>
#include <list>
#include "Particle.hpp"


class Attacker : public Particle
{
public:
    Attacker(
        double x,
        double y
    );

    virtual ~Attacker()=default;

    void update(const std::list<Particle*>& particles) override;

    void move() override;

    bool lockedOn();

    int getWeaponStrength();

    // Fire the weapon.
    void fire();

    // Return a pointer to the target, if there is one.
    Particle* getTarget();

private:
    // Increase the weapon strength by a certain amount.
    void increaseWeaponStrength(double amount);

    Particle* target;
    double ws;
    double angle;
    int lifespan;
    int range;
};

#endif