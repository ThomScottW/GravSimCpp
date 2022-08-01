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

    void move() override;

    // Choose a target to move towards.
    void selectTarget(std::list<Particle> particles);

    // Fire the weapon.
    void fire();

private:
    // Increase the weapon strength by a certain amount.
    void increaseWeaponStrength(int amount);

    int weaponStrength;
};

#endif