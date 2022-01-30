#include "Environment.hpp"


Environment::Environment(unsigned numParticles)
    : numParticles{numParticles}
{
    particles = std::list<Particle>();

    for (unsigned i = 0; i < numParticles; ++i){
        particles.push_back(genRandomParticle());
    }
}


void Environment::update()
{
    // Remove any absorbed particles, or particles outside the screen.
    for (auto it = particles.begin(); it != particles.end();)
    {
        if ((*it).isAbsorbed() || isOutsideBounds(*it))
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
                (*pIt1).accelerateTowards((*pIt2).x(), (*pIt2).y(), (*pIt2).getMass());
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
    double mass = std::pow(radius, 2);

    // 0 Motion Vector.
    MotionVector<double> vec = MotionVector<double>(0, 0);

    return Particle(radius, x, y, mass, vec);
}


void Environment::placeParticle(Particle p)
{
    particles.push_back(p);
}


std::list<Particle>& Environment::getParticles()
{
    return particles;
}


std::vector<unsigned> Environment::dimensions()
{
    std::vector<unsigned> dim = {width, height};
    return dim;
}


bool Environment::isOutsideBounds(Particle p)
{
    bool outX = false;
    bool outY = false;

    // Check if outside x bounds.
    if (p.x() < -p.getRadius() || p.x() > width + p.getRadius())
    {
        std::cout << "Particle is out of x bounds." << std::endl;
        outX = true;
    }

    // Check if outside y bounds.
    if (p.y() < -p.getRadius() || p.y() > height + p.getRadius())
    {
        std::cout << "Particle is out of y bounds." << std::endl;
        outY = true;
    }

    return outX || outY;
}
