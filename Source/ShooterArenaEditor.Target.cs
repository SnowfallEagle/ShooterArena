// ShooterArena Game, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShooterArenaEditorTarget : TargetRules
{
	public ShooterArenaEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ShooterArena" } );
	}
}
