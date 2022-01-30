#ifndef PARTICLE_HPP
#define PARTICLE_HPP


#include "MotionVector.hpp"


class Particle
{
public:
    Particle(
        double radius,
        double x,
        double y,
        double mass,
        MotionVector<double> vec
    );

    // Return the x coordinate of this particle.
    double x();

    // Return the y coordinate of this particle.
    double y();
    
    // Move the particle based on its vector's direction and speed.
    void move();

    // Accelrate this particle towards a point.
    void accelerateTowards(double x, double y, double pointMass=1);

    // Collide with another particle, coalescing into a larger particle.
    void coalesce(Particle& p2);

    // Return the distance from this particle's center to a point.
    double distanceFrom(double x, double y);

    // Return true if this particle is colliding with p2.
    bool isCollidingWith(Particle& p2);

    // Return true if this particle has been absorbed by another.
    bool isAbsorbed();

    // Return the radius of a particle with a given mass.
    double calculateRadius(double mass);

    // Return the radius of this particle.
    double getRadius();

    // Return the mass of this particle.
    double getMass();

    // Simulate the effect of elasticity by applying a force to the particle's
    // motion vector.
    void applyElasticity();


private:
    double rad;
    double x_pos;
    double y_pos;
    double mass;
    MotionVector<double> vec;
    bool absorbed;
};


#endif
