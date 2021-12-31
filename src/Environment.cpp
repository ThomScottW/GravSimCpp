#include "Environment.hpp"


Environment::Environment(unsigned numParticles)
    : numParticles{numParticles}
{
    particles = std::vector<Particle>();

    for (unsigned i = 0; i < numParticles; ++i){
        particles.push_back(genRandomParticle());
    }
}


void Environment::update()
{
    // Remove any absorbed particles.
    for (auto it = particles.begin(); it != particles.end();)
    {
        if ((*it).isAbsorbed())
        {
            it = particles.erase(it);
        }
        else
        {
            ++it;
        }
    }


    for (auto pIt1 = particles.begin(); pIt1 != particles.end(); ++pIt1)
    {
        (*pIt1).move();

        for (auto pIt2 = particles.begin(); pIt2 != particles.end(); ++pIt2)
        {
            if (pIt1 != pIt2)
            {
                (*pIt1).accelerateTowards((*pIt2).coordinates(), (*pIt2).getMass());
                (*pIt1).coalesce(*pIt2);
            }
        }
    }
}


Particle Environment::genRandomParticle()
{
    double radius = std::rand() % 10 + 5;

    // Random positions that will fit within the window.
    double x = std::rand() % width;
    double y = std::rand() % height;

    std::vector<double> position = {x, y};

    double mass = std::pow(radius, 2);

    // 0 Motion Vector.
    MotionVector<double> vec = MotionVector<double>(0, 0);

    return Particle(radius, position, mass, vec);
}


std::vector<Particle>& Environment::getParticles()
{
    return particles;
}


std::vector<unsigned> Environment::dimensions()
{
    std::vector<unsigned> dim = {width, height};
    return dim;
}