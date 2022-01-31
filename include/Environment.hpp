#ifndef ENV_HPP
#define ENV_HPP


#include <cstdlib>
#include <list>
#include <vector>
#include <iostream>
#include "Particle.hpp"


class Environment
{
public:
    // Constructor.
    Environment(unsigned numParticles=10);

    // Update the environment to move particles, resolve collisions, etc.
    void update();

    // Generate a random particle.
    Particle genRandomParticle();

    // Place a particle into the environment.
    void placeParticle(Particle p);

    // Given some coordinates, return the particle at the coordinates, if any.
    Particle* findParticle(double x, double y);

    // Return a reference to the list of particles in this environment.
    std::list<Particle>& getParticles();

    // Return a vector containing width and height of this environment.
    std::vector<unsigned> dimensions();

    // double calcOrbitalVelocity(double m1, double m2, )

    static constexpr double GRAVITATIONAL_CONSTANT = 0.0001;
    static constexpr double ELASTICITY_CONSTANT = 0.9;


private:

    // Returns true if particle p is out of bounds.
    bool isOutsideBounds(Particle p);

    unsigned numParticles;
    std::list<Particle> particles;
    unsigned width = 1200;
    unsigned height = 1200;

};


#endif
