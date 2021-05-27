// By Adamo Maiorano (2021)

using UnrealBuildTool;
using System.Collections.Generic;

public class TheLookoutTarget : TargetRules
{
	public TheLookoutTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "TheLookout" } );
	}
}
