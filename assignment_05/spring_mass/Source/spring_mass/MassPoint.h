// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class SPRING_MASS_API MassPoint
{
	// grant direct access for Spring
	friend class Spring;
protected:
	// index of the vertex this mass point is attached to
	uint32 m_vertex_id;
	// current position in local coordinate system
	FVector m_currPos;
	// current mass-spring system state
	FVector m_force, m_velocity;

	// is this point not fixed?
	bool m_movable;
	// mass parameter
	float m_mass;

public:
	MassPoint(uint32 vertex_id, bool movable, FVector pos);
	~MassPoint();

	// apply gravitational force
	void updateGravity();
	// update velocity and position
	void updateCurPos(float deltaT);
	// add an external force
	void addForce(FVector f);

	uint32 getVertexId() { return m_vertex_id; };
	FVector getCurrPos() { return m_currPos; };
};
