// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "vr02_lag_cpp.h"
#include "vr02_lag_cppGameMode.h"
#include "vr02_lag_cppCharacter.h"

Avr02_lag_cppGameMode::Avr02_lag_cppGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
