#include "Particle.hpp"


Particle::Particle(
    double radius,
    double x,
    double y,
    MotionVector<double> vec,
    SDL_Color col,
    bool gravity,
    double density
)
    : rad{radius},
    x_pos{x},
    y_pos{y},
    vec{vec},
    density{density},
    absorbed{false},
    fixed{false},
    hasGrav{gravity},
    color{col}
{
    mass = calcMass(radius, density);
}


double Particle::x()
{
    return x_pos;
}


double Particle::y()
{
    return y_pos;
}


void Particle::update(const std::list<Particle*>& particles)
{
    move();
    for (Particle* p : particles)
    {
        if (p != this && p->hasGravity())
        {
            accelerateTowards(p->x(), p->y(), GRAVITATIONAL_CONSTANT, p->getMass());
            coalesce(*p, ELASTICITY_CONSTANT);
        }
    }
}


void Particle::move()
{
    if (fixed)
    {
        return;
    }
    
    // Get the angle and magnitude of this particle's motion vector.
    double vec_angle = vec.angle();
    double vec_mag = vec.magnitude();

    // Calculate the amount that this particle will travel in both the x and y direction.
    double dx = vec_mag * std::cos(vec_angle);
    double dy = vec_mag * std::sin(vec_angle);

    // Move the particle by the calculated amounts.
    x_pos += dx * (1.0 / 60.0);
    y_pos += dy * (1.0 / 60.0);
}


void Particle::accelerateTowards(double x, double y, double constant, double pointMass)
{
    if (fixed)
    {
        return;
    }

    double dx = x - x_pos;
    double dy = y - y_pos;
    double dist = std::hypot(dx, dy);

    double angleBetweenPoints = std::atan2(dy, dx);

    // The gravitational equation.
    // This calculated force will be the magnitude of the vector that we'll add to this
    // particle's existing motion vector, simulating an acceleration towards the point.
    double force = (constant * mass * pointMass) / std::pow(dist, 2);
    double acceleration = force / mass;
    double deltaVel = acceleration * (1.0/60.0);

    double velXComp = std::cos(angleBetweenPoints) * deltaVel;
    double velYComp = std::sin(angleBetweenPoints) * deltaVel;

    // Add the force vector to this particle's motion vector.
    MotionVector<double> forceVec(velXComp, velYComp);
    vec = vec + forceVec;
}


void Particle::coalesce(Particle& p2, double constant)
{
    if (!isCollidingWith(p2))
    {
        return;
    }

    double totalMass = mass + p2.mass;

    // Case where p1 is larger and absorbs p2.
    if (mass >= p2.mass)
    {
        // Create a new position and vector that is based on the masses
        // of the particles.
        x_pos = (x_pos * mass + p2.x_pos * p2.mass) / totalMass;

        // The larger particle should stay moving in roughly the same direction,
        // but can be diverted slightly by the smaller particle.
        double smallerVecMagnitude = (p2.vec.magnitude() * p2.mass) / totalMass;
        double smallerVecAngle = p2.vec.angle();
        double smallerVecX = std::cos(smallerVecAngle) * smallerVecMagnitude;
        double smallerVecY = std::sin(smallerVecAngle) * smallerVecMagnitude;

        // Add this new weighted vector to p1's existing vector.
        vec = vec + MotionVector<double>(smallerVecX, smallerVecY);

        // Make it seem like energy was lost during the collision.
        applyElasticity(constant);

        // Update mass and change radius accordingly.
        mass = totalMass;
        rad = calcRad(totalMass, density);

        // Set this flag to true so the simulation can remove p2.
        p2.absorbed = true;
    }
    // Case where p2 is larger.
    else if (mass < p2.mass)
    {
        p2.x_pos = (p2.x_pos * p2.mass + x_pos * mass) / totalMass;

        double smallerVecMagnitude = (vec.magnitude() * mass) / totalMass;
        double smallerVecAngle = vec.angle();
        double smallerVecX = std::cos(smallerVecAngle) * smallerVecMagnitude;
        double smallerVecY = std::sin(smallerVecAngle) * smallerVecMagnitude;

        p2.vec = p2.vec + MotionVector<double>(smallerVecX, smallerVecY);

        p2.applyElasticity(constant);

        p2.mass = totalMass;
        p2.rad = Particle::calcRad(totalMass, density);

        absorbed = true;
    }
}


double Particle::distanceFrom(double x, double y)
{
    return std::hypot(x - x_pos, y - y_pos);
}


bool Particle::isCollidingWith(Particle& p2)
{
    return (distanceFrom(p2.x_pos, p2.y_pos) < rad + p2.rad);
}


bool Particle::isAbsorbed()
{
    return absorbed;
}


void Particle::freeze()
{
    fixed = true;
    vec.setX(0);
    vec.setY(0);
}


void Particle::unFreeze()
{
    fixed = false;
}


bool Particle::isFrozen()
{
    return fixed;
}


bool Particle::hasGravity()
{
    return hasGrav;
}


double Particle::calcRad(double mass, double density)
{
    return std::cbrt((3.0 / (4.0 * density * M_PI)) * mass);
}


double Particle::calcMass(double radius, double density)
{
    return (4.0 / 3.0) * M_PI * std::pow(radius, 3) * density;
}


double Particle::getRadius()
{
    return rad;
}


double Particle::getMass()
{
    return mass;
}


void Particle::changeMass(double amount)
{
    mass += amount;
}


MotionVector<double> Particle::getVelocity()
{
    return vec;
}


SDL_Color Particle::getColor()
{
    return color;
}


void Particle::applyElasticity(double constant)
{
    vec = vec * constant;
}
