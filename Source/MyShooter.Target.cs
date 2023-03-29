// MyShooter Game, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyShooterTarget : TargetRules
{
	public MyShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;

        ExtraModuleNames.AddRange( new string[] { "MyShooter" } );
	}
}
