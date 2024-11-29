// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class vr02_lag_cppEditorTarget : TargetRules
{
	public vr02_lag_cppEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("vr02_lag_cpp");
	}

}
