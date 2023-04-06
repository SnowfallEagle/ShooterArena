// ShooterArena Game, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShooterArenaTarget : TargetRules
{
	public ShooterArenaTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange( new string[] { "ShooterArena" } );
	}
}
