// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MassPoint.h"

/**
 * 
 */
class SPRING_MASS_API Spring
{
protected:
	// mass points connected by spring
	MassPoint *m_m1;
	MassPoint *m_m2;

	// length of the springs
	float m_spring_length_init;

	float m_stiffness;
	float m_damper;

public:
	Spring(MassPoint* m1, MassPoint* m2, float length);
	~Spring();
	void Tick();
};
