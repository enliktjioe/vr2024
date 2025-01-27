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
}

void 
MassPoint::updateCurPos(float deltaT)
{
	if (m_movable)
	{
		// FIXME: Verlet
		m_velocity += m_force / m_mass * deltaT;
		m_currPos += m_velocity * deltaT;
	}
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
