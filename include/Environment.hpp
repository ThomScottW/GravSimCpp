#ifndef ENV_HPP
#define ENV_HPP


#include <cstdlib>
#include <list>
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

    // Return a reference to the list of particles in this environment.
    std::vector<Particle>& getParticles();

    // Return a vector containing width and height of this environment.
    std::vector<unsigned> dimensions();



    

private:
    unsigned numParticles;
    std::vector<Particle> particles;
    unsigned width = 800;
    unsigned height = 800;

};


#endif