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

    void move() override;

    // Choose a target to move towards.
    void selectTarget(Particle* target);

    // Fire the weapon.
    void fire();

private:
    // Increase the weapon strength by a certain amount.
    void increaseWeaponStrength(int amount);

    Particle* target;
    int weaponStrength;
    double angle;
    int lifespan;
};

#endif