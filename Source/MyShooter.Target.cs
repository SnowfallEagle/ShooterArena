// MyShooter Game, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyShooterTarget : TargetRules
{
	public MyShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange( new string[] { "MyShooter" } );
	}
}
