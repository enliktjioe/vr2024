// Fill out your copyright notice in the Description page of Project Settings.

#include "MassPoint.h"
#include "spring_mass.h"


MassPoint::MassPoint(uint32 vertex_id, bool movable, FVector pos) :
m_vertex_id(vertex_id),
m_currPos(pos),
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

	// Define the gravitational acceleration (assuming Earth, in cm/s^2)
    const FVector gravity(0.0f, 0.0f, -980.0f); // Negative Z-direction for gravity

    // Calculate gravitational force: F = m * g
    FVector gravitationalForce = m_mass * gravity;

    // Add gravitational force to the current force
    addForce(gravitationalForce);
}

void 
MassPoint::updateCurPos(float deltaT)
{
	if (m_movable)
	{
		// Store the current position before updating
		FVector newPos = m_currPos + (m_currPos - m_prevPos) + (m_force / m_mass) * (deltaT * deltaT);

		// Update the previous position to the current position
		m_prevPos = m_currPos;

		// Update the current position to the new position
		m_currPos = newPos;
	}

	// Reset the accumulated force for the next time step
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
