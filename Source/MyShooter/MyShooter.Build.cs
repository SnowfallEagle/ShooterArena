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
                "GameplayTasks",
                "NavigationSystem",
            }
        );

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[]
            {
                "MyShooter/Public/Components",

                "MyShooter/Public/Character",
                "MyShooter/Public/Character/Components",

                "MyShooter/Public/Player",
                "MyShooter/Public/Dev",

                "MyShooter/Public/Weapon",
                "MyShooter/Public/Weapon/Components",

                "MyShooter/Public/UI",
                "MyShooter/Public/Animations",
                "MyShooter/Public/Pickups",

                "MyShooter/Public/AI",
                "MyShooter/Public/AI/Components",
                "MyShooter/Public/AI/Tasks",
                "MyShooter/Public/AI/Services",
                "MyShooter/Public/AI/EQS",
                "MyShooter/Public/AI/Decorators",

                "MyShooter/Public/Menu",
                "MyShooter/Public/Menu/UI",
            }
        );

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
