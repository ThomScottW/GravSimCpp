#ifndef ENV_HPP
#define ENV_HPP


#include <cstdlib>
#include <list>
#include <vector>
#include <iostream>
#include "Particle.hpp"
#include "EnvConstants.hpp"


class Environment
{
public:
    // Constructor.
    Environment(unsigned numParticles=10);

    // Update the environment to move particles, resolve collisions, etc.
    void update();

    // Generate a random particle.
    Particle* genRandomParticle();

    // Place a particle into the environment. A bool value determines if it
    // can be influeced by gravity.
    void placeParticle(Particle* p, bool gravity=true);

    // Given some coordinates, return the particle at the coordinates, if any.
    Particle* findParticle(double x, double y);

    // Return a reference to the list of particles in this environment.
    std::list<Particle*>& getParticles();

    // Get the list of particles that don't interact with gravity.
    std::list<Particle*>& getNonGravityParticles();

    // Return a vector containing width and height of this environment.
    std::vector<unsigned> dimensions();


private:
    // Returns true if particle p is out of bounds.
    bool isOutsideBounds(Particle p);

    unsigned numParticles;
    std::list<Particle*> particles;
    std::list<Particle*> nonAttracted;
    unsigned width = 1000;
    unsigned height = 1000;

};


#endif
