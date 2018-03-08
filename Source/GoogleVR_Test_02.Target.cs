// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class GoogleVR_Test_02Target : TargetRules
{
	public GoogleVR_Test_02Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "GoogleVR_Test_02" } );
	}
}
