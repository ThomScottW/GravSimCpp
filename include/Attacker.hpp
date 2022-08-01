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

    // Fire the weapon.
    void fire();

private:
    // Increase the weapon strength by a certain amount.
    void increaseWeaponStrength(int amount);

    Particle* target;
    int weaponStrength;
    double angle;
    int lifespan;
    int range;
};

#endif