// Fill out your copyright notice in the Description page of Project Settings.

#include "Spring.h"
#include "spring_mass.h"
#include "MassPoint.h"

Spring::Spring(MassPoint* m1, MassPoint* m2, float length, float restLength) :
m_m1(m1), m_m2(m2), 
m_spring_length_init(length),
m_restLength(restLength),
m_stiffness(0.1),
m_damper(0.001)
{
}

void Spring::Tick()
{	
	//FIXME: Force
	//m_m1->m_currPos.Normalize()
	//FVector::Dist()

    // Get the positions of the connected mass points
    FVector pos1 = m_m1->getCurrPos();
    FVector pos2 = m_m2->getCurrPos();

    // Calculate the vector between the two points
    FVector delta = pos2 - pos1;

    // Calculate the current length of the spring
    float currentLength = delta.Size();

    // Calculate the displacement from the rest length
    float displacement = currentLength - m_restLength;

    // Normalize the direction of the spring force
    FVector direction = delta / currentLength;

    // Calculate the spring force using Hooke's Law: F = -k * displacement
    FVector springForce = -m_stiffness * displacement * direction;

    // Add damping force to reduce oscillations: F_damping = -d * v_relative
    FVector relativeVelocity = m_m2->getVelocity() - m_m1->getVelocity();
    FVector dampingForce = -m_damping * FVector::DotProduct(relativeVelocity, direction) * direction;

    // Total force on the spring
    FVector totalForce = springForce + dampingForce;

    // Apply the force to the mass points
    m_m1->addForce(totalForce);
    m_m2->addForce(-totalForce); // Opposite force to the second mass point
}
