// By Adamo Maiorano (2021)

using UnrealBuildTool;
using System.Collections.Generic;

public class TheLookoutEditorTarget : TargetRules
{
	public TheLookoutEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "TheLookout" } );
	}
}
