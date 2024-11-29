// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class vr02_lag_cpp : ModuleRules
{
	public vr02_lag_cpp(ReadOnlyTargetRules Target) : base (Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
