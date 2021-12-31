#include "Particle.hpp"


namespace{
    double GRAVITATIONAL_CONSTANT = 0.0001;
    double ELASTICITY_CONSTANT = 0.9;
}


Particle::Particle(
    double radius,
    std::vector<double> position,
    double mass,
    MotionVector<double> vec
)
    : rad{radius}, x_pos{position[0]}, y_pos{position[1]}, mass{mass}, vec{vec}, absorbed{false}
{
}


std::vector<double> Particle::coordinates()
{
    std::vector<double> coords = {x_pos, y_pos};
    return coords;
}


void Particle::move()
{
    // Get the angle and magnitude of this particle's motion vector.
    double vec_angle = vec.angle();
    double vec_mag = vec.magnitude();

    // Calculate the amount that this particle will travel in both the x and y direction.
    double dx = vec_mag * std::cos(vec_angle);
    double dy = vec_mag * std::sin(vec_angle);

    // Move the particle by the calculated amounts.
    x_pos += dx;
    y_pos += dy;
}


void Particle::accelerateTowards(std::vector<double> point, double pointMass)
{
    double dx = point[0] - x_pos;
    double dy = point[1] - y_pos;
    double dist = std::hypot(dx, dy);

    double angleBetweenPoints = std::atan2(dy, dx);

    // The gravitational equation.
    // This calculated force will be the magnitude of the vector that we'll add to this
    // particle's existing motion vector, simulating an acceleration towards the point.
    double force = (GRAVITATIONAL_CONSTANT * mass * pointMass) / std::pow(dist, 2);

    double forceXComp = std::cos(angleBetweenPoints) * force;
    double forceYComp = std::sin(angleBetweenPoints) * force;

    // Add the force vector to this particle's motion vector.
    MotionVector<double> forceVec(forceXComp, forceYComp);
    vec = vec + forceVec;
}


void Particle::coalesce(Particle& p2)
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
        applyElasticity();

        // Update mass and change radius accordingly.
        mass = totalMass;
        rad = calculateRadius(mass);

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

        p2.applyElasticity();

        p2.mass = totalMass;
        p2.rad = calculateRadius(totalMass);

        absorbed = true;
    }
}


double Particle::distanceFrom(std::vector<double> point)
{
    return std::hypot(point[0] - x_pos, point[1] - y_pos);
}


bool Particle::isCollidingWith(Particle& p2)
{
    return (distanceFrom(p2.coordinates()) < rad + p2.rad);
}


bool Particle::isAbsorbed()
{
    return absorbed;
}


double Particle::calculateRadius(double mass)
{
    return std::sqrt(mass);
}


double Particle::getRadius()
{
    return rad;
}


double Particle::getMass()
{
    return mass;
}


void Particle::applyElasticity()
{
    vec = vec * ELASTICITY_CONSTANT;
}
