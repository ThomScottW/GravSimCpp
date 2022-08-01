#include "Environment.hpp"


Environment::Environment(unsigned numParticles)
    : numParticles{numParticles}
{
    particles = std::list<Particle*>();

    for (unsigned i = 0; i < numParticles; ++i){
        particles.push_back(genRandomParticle());
    }
}


Environment::~Environment()
{
    // Delete the particles array.
    while (!particles.empty())
    {
        delete particles.front();
        particles.pop_front();
    }
}


void Environment::update()
{
    // Remove any absorbed particles, or particles outside the screen.
    for (auto it = particles.begin(); it != particles.end();)
    {
        if ((*it)->isAbsorbed() || isOutsideBounds(*(*it)))
        {
            delete (*it); // Free up dynamically allocated memory.
            it = particles.erase(it);
        }
        else
        {
            ++it;
        }
    }


    for (Particle* p : particles)
    {
        p->update(particles);
    }


    // // Gravity.
    // for (auto pIt1 = particles.begin(); pIt1 != particles.end(); ++pIt1)
    // {
    //     (*pIt1)->move();

    //     for (auto pIt2 = particles.begin(); pIt2 != particles.end(); ++pIt2)
    //     {
    //         if (pIt1 != pIt2)
    //         {
    //             (*pIt1)->accelerateTowards((*pIt2)->x(), (*pIt2)->y(),
    //              GRAVITATIONAL_CONSTANT, (*pIt2)->getMass());
    //             (*pIt1)->coalesce(*(*pIt2), ELASTICITY_CONSTANT);
    //         }
    //     }
    // }
}


Particle* Environment::genRandomParticle()
{
    double radius = std::rand() % 5 + 1;

    // Random positions that will fit within the window.
    double x = std::rand() % width;
    double y = std::rand() % height;

    // 0 Motion Vector.
    MotionVector<double> vec = MotionVector<double>(0, 0);

    return new Particle(radius, x, y, vec);
}


void Environment::placeParticle(Particle* p)
{
    particles.push_back(p);
}


Particle* Environment::findParticle(double x, double y)
{
    for (auto it = particles.begin(); it != particles.end(); ++it)
    {
        if (std::hypot(x - (*it)->x(), y - (*it)->y()) <= (*it)->getRadius())
        {
            return (*it);
        }
    }

    return nullptr;
}


std::list<Particle*>& Environment::getParticles()
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
