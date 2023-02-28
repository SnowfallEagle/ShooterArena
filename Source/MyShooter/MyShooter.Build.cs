// MyShooter Game, All Rights Reserved.

using UnrealBuildTool;

public class MyShooter : ModuleRules
{
    public MyShooter(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Niagara",
                "PhysicsCore",
            }
        );

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[]
            {
                "MyShooter/Public/Components",
                "MyShooter/Public/Player",
                "MyShooter/Public/Dev",
                "MyShooter/Public/Weapon",
                "MyShooter/Public/Weapon/Components",
                "MyShooter/Public/UI",
                "MyShooter/Public/Animations",
                "MyShooter/Public/Pickups",
            }
        );
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
