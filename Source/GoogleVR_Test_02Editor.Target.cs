// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class GoogleVR_Test_02EditorTarget : TargetRules
{
	public GoogleVR_Test_02EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "GoogleVR_Test_02" } );
	}
}
