// Fill out your copyright notice in the Description page of Project Settings.

#include "Spring.h"
#include "spring_mass.h"

Spring::Spring(MassPoint* m1, MassPoint* m2, float length) :
m_m1(m1), m_m2(m2), 
m_spring_length_init(length),
m_stiffness(0.1),
m_damper(0.001)
{
}

Spring::~Spring()
{
}


void
Spring::Tick()
{	
	//FIXME: Force
	//m_m1->m_currPos.Normalize()
	//FVector::Dist()

    // Calculate the vector between the two mass points
    FVector displacement = m_m2->m_currPos - m_m1->m_currPos;
    float currentLength = displacement.Size();

    // Normalize the displacement vector to get the direction
    FVector direction = displacement / currentLength;

    // Hooke's Law: Spring force
    float springForceMagnitude = -m_stiffness * (currentLength - m_spring_length_init);
    FVector springForce = springForceMagnitude * direction;

    // Relative velocity of the two mass points
    FVector relativeVelocity = m_m2->m_velocity - m_m1->m_velocity;

    // Damping force
    float dampingForceMagnitude = -m_damper * FVector::DotProduct(relativeVelocity, direction);
    FVector dampingForce = dampingForceMagnitude * direction;

    // Total force
    FVector totalForce = springForce + dampingForce;

    // Apply forces to the mass points
    m_m1->addForce(-totalForce); // Apply the force in the opposite direction to m_m1
    m_m2->addForce(totalForce);  // Apply the force to m_m2
}
