#ifndef PARTICLE_HPP
#define PARTICLE_HPP


#include <iostream>
#include <list>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "MotionVector.hpp"
#include "EnvConstants.hpp"


class Particle
{
public:
    Particle(
        double radius,
        double x,
        double y,
        MotionVector<double> vec,
        SDL_Color col=SDL_Color{255, 255, 255},
        bool gravity=true,
        double density=5500
    );

    virtual ~Particle()=default;

    // Return the x coordinate of this particle.
    double x();

    // Return the y coordinate of this particle.
    double y();

    // Move and accelerate towards other particles.
    virtual void update(std::list<Particle*>& particles);
    
    // Move the particle based on its vector's direction and speed.
    virtual void move();

    // Accelrate this particle towards a point.
    void accelerateTowards(double x, double y, double constant, double pointMass=1);

    // Collide with another particle, coalescing into a larger particle. An elasticity
    // constant simulates the loss of energy after collision.
    void coalesce(Particle& p2, double constant);

    // Explode this particle.
    void explode(std::list<Particle*>& particles);

    // Return the distance from this particle's center to a point.
    double distanceFrom(double x, double y);

    // Return true if this particle is colliding with p2.
    bool isCollidingWith(Particle& p2);

    // Return true if this particle has been absorbed by another.
    bool isAbsorbed();

    // Stops the particle from being affected by gravity and freezes it in place.
    void freeze();

    // The particle can now be affected by gravity.
    void unFreeze();

    // Return true if the particle is frozen.
    bool isFrozen();

    // Return true if this particle is one that interacts through gravity.
    bool hasGravity();

    // Return the radius of a particle with a given mass.
    double static calcRad(double mass, double density);

    // Return the mass of a particle, given a radius.
    double static calcMass(double radius, double density);

    // Return the radius of this particle.
    double getRadius();

    // Return the mass of this particle.
    double getMass();

    // Change the mass of this particle by a set amount.
    void changeMass(double amount);

    // Return the motion vector that represents the velocity of this particle.
    MotionVector<double> getVelocity();

    // Return an SDL_Color struct representing the r, g, and b values of this particle's color.
    // The color starts off as white and turns orange if the particle is low mass.
    virtual SDL_Color getColor();

    // Simulate the effect of elasticity by applying a force to the particle's
    // motion vector. The elasticity constant is defined by the environment.
    void applyElasticity(double constant);



// The protected keyword acts like private, but now derived classes
// can modify these member variables directly.
protected:
    double rad;
    double oriRad;
    double x_pos;
    double y_pos;
    double mass;
    double oriMass;
    MotionVector<double> vec;
    double density;
    bool absorbed;
    bool fixed;
    bool hasGrav;
    SDL_Color color;
};


#endif
