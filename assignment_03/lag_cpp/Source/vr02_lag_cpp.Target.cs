// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class vr02_lag_cppTarget : TargetRules
{
	public vr02_lag_cppTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.Add("vr02_lag_cpp");
	}
}
