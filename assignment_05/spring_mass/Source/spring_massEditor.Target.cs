// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class spring_massEditorTarget : TargetRules
{
	public spring_massEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
        	ExtraModuleNames.Add("spring_mass");

		// Update build settings
        	DefaultBuildSettings = BuildSettingsVersion.Latest;

        	// Update include order version
        	IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
	}
}
