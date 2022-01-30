#ifndef MOTIONVEC_HPP
#define MOTIONVEC_HPP


#include <cmath>


template <typename CompType>
class MotionVector
{
public:
    // Constructor that allows us to create a MotionVector with no arguments.
    // Initalizes the x and y components to be 0.
    MotionVector();

    // Constructor that takes the x and y components.
    MotionVector(CompType x, CompType y);

    // Copy Constructor
    template <typename OtherCompType>
    MotionVector(const MotionVector<OtherCompType>& other);

    // Assignment Operator
    template <typename OtherCompType>
    MotionVector& operator=(const MotionVector<OtherCompType>& other);

    // Return the x component of this MotionVector.
    CompType x() const;

    // Return the y component of this MotionVector.
    CompType y() const;

    // Set the x component of this MotionVector.
    void setX(CompType newX);

    // Set the y component of this MotionVector.
    void setY(CompType newY);

    // Return the magnitude of this MotionVector.
    double magnitude();

    // Return a MotionVector pointing in the same direction, but with
    // magnitude of 1.
    MotionVector<CompType> unit();

    // Return the angle in radians that this vector is pointing at.
    double angle();

    // Perform the dot product operation between two MotionVectors.
    template <typename OtherCompType>
    double dotProduct(const MotionVector<OtherCompType>& other);

    // Overload the addition operator to support the addition of two MotionVectors.
    // Note that both vectors must be of the same type.
    template <typename OtherCompType>
    MotionVector<CompType> operator+(const MotionVector<OtherCompType>& other);

    // Overload the multiplication operator to support multiplying the components
    // of the vector by a factor.
    template <typename FacType>
    MotionVector<CompType> operator*(FacType factor);


private:
    CompType x_comp;
    CompType y_comp;
};


template <typename CompType>
MotionVector<CompType>::MotionVector()
    : x_comp{0}, y_comp{0}
{
}


template <typename CompType>
MotionVector<CompType>::MotionVector(CompType x, CompType y)
    : x_comp{x}, y_comp{y}
{
}


template <typename CompType>
template <typename OtherCompType>
MotionVector<CompType>::MotionVector(const MotionVector<OtherCompType>& other)
    : x_comp{other.x()}, y_comp{other.y()}
{
}


template <typename CompType>
template <typename OtherCompType>
MotionVector<CompType>& MotionVector<CompType>::operator=(const MotionVector<OtherCompType>& other)
{
    x_comp = other.x_comp;
    y_comp = other.y_comp;
    return *this;
}


template <typename CompType>
CompType MotionVector<CompType>::x() const
{
    return x_comp;
}


template <typename CompType>
CompType MotionVector<CompType>::y() const
{
    return y_comp;
}


template <typename CompType>
void MotionVector<CompType>::setX(CompType newX)
{
    x_comp = newX;
}


template <typename CompType>
void MotionVector<CompType>::setY(CompType newY)
{
    y_comp = newY;
}


template <typename CompType>
double MotionVector<CompType>::magnitude()
{
    return std::sqrt(std::pow(x_comp, 2) + std::pow(y_comp, 2));
}


template <typename CompType>
MotionVector<CompType> MotionVector<CompType>::unit()
{
    // Initialize a new MotionVector.
    MotionVector<CompType> newMV;

    // Scale the x and y components to make it a unit vector.
    newMV.setX(x_comp / magnitude());
    newMV.setY(y_comp / magnitude());

    return newMV; 
}


template <typename CompType>
double MotionVector<CompType>::angle()
{
    return std::atan2(y_comp, x_comp);
}


template <typename CompType>
template <typename OtherCompType>
double MotionVector<CompType>::dotProduct(const MotionVector<OtherCompType>& other)
{
    return (x_comp * other.x()) + (y_comp * other.y());
}


template <typename CompType>
template <typename OtherCompType>
MotionVector<CompType> MotionVector<CompType>::operator+(const MotionVector<OtherCompType>& other)
{
    MotionVector<CompType> newMV;

    newMV.setX(x_comp + other.x());
    newMV.setY(y_comp + other.y());

    return newMV;
}


template <typename CompType>
template <typename FacType>
MotionVector<CompType> MotionVector<CompType>::operator*(FacType factor)
{
    MotionVector<CompType> newMV;

    newMV.setX(x_comp * factor);
    newMV.setY(y_comp * factor);

    return newMV;
}


#endif
