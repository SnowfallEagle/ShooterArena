// MyShooter Game, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyShooterEditorTarget : TargetRules
{
	public MyShooterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "MyShooter" } );
	}
}
