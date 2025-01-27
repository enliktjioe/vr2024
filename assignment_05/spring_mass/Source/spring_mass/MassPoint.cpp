// Fill out your copyright notice in the Description page of Project Settings.

#include "MassPoint.h"
#include "spring_mass.h"


MassPoint::MassPoint(uint32 vertex_id, bool movable, FVector pos) :
m_vertex_id(vertex_id),
m_currPos(pos),
m_prevPos(pos), // Initialize m_prevPos to m_currPos
m_force(FVector::ZeroVector),
m_velocity(FVector::ZeroVector),
m_movable(movable),
m_mass(0.00005)
{
}

void
MassPoint::updateGravity()
{
	// FIXME: gravity

	// Base gravitational acceleration
	FVector gravity = FVector(0, 0, -9.81);

	// Add random perturbation to each component
	float perturbationRange = 0.1f; // Define the range of the random perturbation
	FVector perturbation(
		FMath::RandRange(-perturbationRange, perturbationRange),
		FMath::RandRange(-perturbationRange, perturbationRange),
		FMath::RandRange(-perturbationRange, perturbationRange)
	);

	FVector perturbedGravity = gravity + perturbation;
	FVector gravityForce = m_mass * perturbedGravity; // F = m * g
	addForce(gravityForce); // Apply the perturbed gravity force
}

void 
MassPoint::updateCurPos(float deltaT)
{
	if (m_movable)
	{
		//// FIXME: Verlet
		//m_velocity += m_force / m_mass * deltaT;
		//m_currPos += m_velocity * deltaT;

		// Compute acceleration (F = m * a => a = F / m)
		FVector acceleration = m_force / m_mass;

		// Verlet integration
		FVector newPos = 2 * m_currPos - m_prevPos + acceleration * FMath::Square(deltaT);

		// Update previous position and current position
		m_prevPos = m_currPos;
		m_currPos = newPos;
	}

	// Reset force for the next iteration
	m_force = FVector::ZeroVector;
}

void
MassPoint::addForce(FVector f)
{
	if (m_movable)
	{
		m_force += f;
	}
}

MassPoint::~MassPoint()
{
}
