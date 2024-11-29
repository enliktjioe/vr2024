// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class VR02_LAG_CPP_API InputEvent
{
	DECLARE_DELEGATE(InputDelegate);
public:
	InputEvent();
	InputEvent(float const start);
	~InputEvent();

	float m_start;
	InputDelegate m_action;
};
