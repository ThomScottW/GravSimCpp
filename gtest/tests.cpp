#include <gtest/gtest.h>
#include "MotionVector.hpp"
#include "Particle.hpp"


TEST(VectorCreationTests, vectorCanBeCreatedWithIntegerCompType)
{
    MotionVector<int> v1(1, 3);

    EXPECT_EQ(v1.x(), 1);
    EXPECT_EQ(v1.y(), 3);
}

TEST(VectorCreationTests, vectorCanBeCreatedWithDoubleCompType)
{
    MotionVector<double> v1(3.5, 78.4);

    EXPECT_EQ(v1.x(), 3.5);
    EXPECT_EQ(v1.y(), 78.4);
}

TEST(VectorSetAttributeTests, canModifyXComponentOfVector)
{
    MotionVector<int> v1(1, 3);
    EXPECT_EQ(v1.x(), 1);

    v1.setX(55);

    EXPECT_EQ(v1.x(), 55);
    EXPECT_EQ(v1.y(), 3);  // Make sure the y component didn't change.
}

TEST(VectorSetAttributeTests, canModifyYComponentOfVector)
{
    MotionVector<int> v1(1, 3);
    EXPECT_EQ(v1.y(), 3);

    v1.setY(55);

    EXPECT_EQ(v1.y(), 55);
    EXPECT_EQ(v1.x(), 1);  // Make sure the x component didn't change.
}

TEST(VectorCreationTests, canCreateMotionVectorUsingCopyConstructor)
{
    MotionVector<int> v1(3, 4);
    EXPECT_EQ(v1.x(), 3);
    EXPECT_EQ(v1.y(), 4);
    
    MotionVector<int> v2(v1);
    
    // Check that the attributes were correctly copied.
    EXPECT_EQ(v2.x(), 3);
    EXPECT_EQ(v2.y(), 4);

    // This is also using the copy constructor.
    MotionVector<int> v3 = v1;
    EXPECT_EQ(v3.x(), 3);
    EXPECT_EQ(v3.y(), 4);

    // Modify v3's values and ensure v1 is unmodified.
    v3.setX(20);
    v3.setY(33);
    EXPECT_EQ(v3.x(), 20);
    EXPECT_EQ(v3.y(), 33);
    EXPECT_EQ(v1.x(), 3);
    EXPECT_EQ(v1.y(), 4);
}

TEST(VectorCreationTests, canCreateMotionVectorsUsingAssignmentOperator)
{
    MotionVector<int> v1(3, 4);
    EXPECT_EQ(v1.x(), 3);
    EXPECT_EQ(v1.y(), 4);

    MotionVector<int> v2(8, 3);
    EXPECT_EQ(v2.x(), 8);
    EXPECT_EQ(v2.y(), 3);

    // Assign v1 into v2.
    v2 = v1;
    
    EXPECT_EQ(v2.x(), 3);
    EXPECT_EQ(v2.y(), 4);

    v2.setX(999);

    EXPECT_EQ(v1.x(), 3);
    EXPECT_EQ(v1.y(), 4);
}

TEST(VectorOperatorTests, canMultiplyIntVectorByFactor)
{
    MotionVector<int> v1(3, 4);

    MotionVector<int> v3 = v1 * 5;

    EXPECT_EQ(v3.x(), 15);
    EXPECT_EQ(v3.y(), 20);
}

TEST(VectorOperatorTests, canMultiplyDoubleVectorByFactor)
{
    MotionVector<double> v1(3.5, 3.89);

    MotionVector<double> v3 = v1 * 5;

    EXPECT_DOUBLE_EQ(v3.x(), 3.5 * 5);
    EXPECT_DOUBLE_EQ(v3.y(), 3.89 * 5);
}

TEST(VectorOperatorTests, canMultiplyIntVectorByDouble)
{
    MotionVector<int> v1(3, 9);

    MotionVector<int> v3 = v1 * 5.1;

    EXPECT_EQ(v3.x(), 3 * 5);
    EXPECT_EQ(v3.y(), 9 * 5);
}

TEST(VectorOperatorTests, canAddTwoIntVectors)
{
    MotionVector<int> v1(3, 4);
    MotionVector<int> v2(9, 12);

    MotionVector<int> v3 = v1 + v2;

    EXPECT_EQ(v3.x(), v1.x() + v2.x());
    EXPECT_EQ(v3.y(), v1.y() + v2.y());
}

TEST(VectorOperatorTests, canAddTwoDoubleVectors)
{
    MotionVector<double> v1(3.123, 4.14);
    MotionVector<double> v2(9.0093, 12.333);

    MotionVector<double> v3 = v1 + v2;

    EXPECT_DOUBLE_EQ(v3.x(), v1.x() + v2.x());
    EXPECT_DOUBLE_EQ(v3.y(), v1.y() + v2.y());
}

TEST(VectorOperatorTests, canAddIntVectorToFloatVector)
{
    MotionVector<double> v1(3.123, 4.14);
    MotionVector<int> v2(3, 1);

    MotionVector<double> v3 = v1 + v2;

    EXPECT_DOUBLE_EQ(v3.x(), v1.x() + v2.x());
    EXPECT_DOUBLE_EQ(v3.y(), v1.y() + v2.y());
}

TEST(VectorDotProductTests, basicDotProduct)
{
    MotionVector<int> v1(3, 4);
    MotionVector<int> v2(10, 11);

    EXPECT_EQ(v1.dotProduct(v2), 3 * 10 + 4 * 11);
}

TEST(VectorDotProductTests, differentTypeDotProduct)
{
    MotionVector<int> v1(3, 4);
    MotionVector<double> v2(6.33, 11.2);

    EXPECT_DOUBLE_EQ(v1.dotProduct(v2), 3 * 6.33 + 4 * 11.2);
}

TEST(VectorAngleTests, basicAngle)
{
    MotionVector<double> v1(3.5, 5.5);

    double expected = std::atan2(5.5, 3.5);

    EXPECT_DOUBLE_EQ(v1.angle(), expected);
}

TEST(VectorAngleTests, piAngle)
{
    MotionVector<double> v1(-1, 0);

    double expected = std::atan2(0, -1);

    EXPECT_DOUBLE_EQ(v1.angle(), expected);
}

TEST(VectorUnitMagnitudeTests, canCreateUnitVector)
{
    MotionVector<double> v1(3, 4);

    double expectedX = 3.0 / 5.0;
    double expectedY = 4.0 / 5.0;

    MotionVector<double> unitV1 = v1.unit();

    EXPECT_DOUBLE_EQ(unitV1.x(), expectedX);
    EXPECT_DOUBLE_EQ(unitV1.y() ,expectedY);
}

TEST(VectorUnitMagnitudeTests, unitVectorHasMagnitudeOf1)
{
    MotionVector<double> v1(3.245234, 4.122);

    MotionVector<double> unitV1 = v1.unit();

    EXPECT_DOUBLE_EQ(unitV1.magnitude(), 1);
}

TEST(VectorUnitMagnitudeTests, largeDecimalUnitVector)
{
    MotionVector<double> v1(383123.1231, 98091.23);

    double expectedX = 383123.1231 / 395480.9943;
    double expectedY = 98091.23 / 395480.9943;

    MotionVector<double> unitV1 = v1.unit();

    // The third argument is the maximum error.
    EXPECT_NEAR(unitV1.x(), expectedX, 0.0000001);
    EXPECT_NEAR(unitV1.y() ,expectedY, 0.0000001);
}

TEST(ParticleCoordinateTests, canReadParticleCoordinates)
{
    // Define basic particle attributes.
    double radius = 1;
    double x = 1;
    double y = 1;
    MotionVector<double> v1(5, 5);

    // Create the particle.
    Particle p1(radius, x, y, v1);

    std::vector<double> expectedPosition = {1, 1};

    std::vector<double> coordinates = {p1.x(), p1.y()};

    EXPECT_EQ(coordinates, expectedPosition);
}

TEST(ParticleCoordinateTests, particlePositionChangesAfterMoving)
{
    // Define basic particle attributes.
    double radius = 1;
    double x = 1;
    double y = 1;
    MotionVector<double> v1(5, 5);

    // Create the particle.
    Particle p1(radius, x, y, v1);

    std::vector<double> expectedPosition = {
        x + v1.magnitude() * std::cos(v1.angle()) * (1.0 / 60.0),
        y + v1.magnitude() * std::sin(v1.angle()) * (1.0 / 60.0)
    };

    p1.move();

    std::vector<double> newPos = {p1.x(), p1.y()};

    EXPECT_EQ(newPos, expectedPosition); 
}

TEST(ParticleCoordinateTests, particlePositionChangesAfterSimpleMove)
{
    // Define basic particle attributes.
    double radius = 1;
    double x = 1;
    double y = 1;
    MotionVector<double> v1(5, 0);

    // Create the particle.
    Particle p1(radius, x, y, v1);

    std::vector<double> expectedPosition = {x + v1.x() / 60, 1};

    p1.move();

    std::vector<double> newPos = {p1.x(), p1.y()};

    EXPECT_EQ(newPos, expectedPosition); 
}

TEST(ParticleCollisionTests, particleCollisionDetected)
{
    // Define basic particle attributes.
    double radius = 20;
    MotionVector<double> v1(0, 0);

    // Create the particle.
    Particle p1(radius, 100, 100, v1);


    // Define basic particle attributes.
    double radius2 = 20;
    MotionVector<double> v2(0, 0);

    // Create the particle.
    Particle p2(radius2, 100, 110, v2);

    ASSERT_TRUE(p1.isCollidingWith(p2));
}

TEST(ParticleCollisionTests, particlesNotCollidingAreNotFlaggedAsColliding)
{
    // Define basic particle attributes.
    double radius = 20;
    MotionVector<double> v1(0, 0);

    // Create the particle.
    Particle p1(radius, 100, 100, v1);


    // Define basic particle attributes.
    double radius2 = 20;
    MotionVector<double> v2(0, 0);

    // Create the particle.
    Particle p2(radius2, 100, 300, v2);

    ASSERT_FALSE(p1.isCollidingWith(p2));
}

TEST(ParticleCollisionTests, particlesJustTouchingAreNotColliding)
{
    // Define basic particle attributes.
    double radius = 20;
    MotionVector<double> v1(0, 0);

    // Create the particle.
    Particle p1(radius, 100, 100, v1);


    // Define basic particle attributes.
    double radius2 = 20;
    MotionVector<double> v2(0, 0);

    // Create the particle.
    Particle p2(radius2, 100, 140, v2);

    ASSERT_FALSE(p1.isCollidingWith(p2));
}
