// ShooterArena Game, All Rights Reserved.

using UnrealBuildTool;

public class ShooterArena : ModuleRules
{
    public ShooterArena(ReadOnlyTargetRules Target) : base(Target)
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
                "ShooterArena/Public/Character",
                "ShooterArena/Public/Character/Components",
                "ShooterArena/Public/Player",

                "ShooterArena/Public/Weapon",
                "ShooterArena/Public/Weapon/Components",

                "ShooterArena/Public/UI",
                "ShooterArena/Public/Animations",
                "ShooterArena/Public/Pickups",

                "ShooterArena/Public/AI",
                "ShooterArena/Public/AI/Components",
                "ShooterArena/Public/AI/Tasks",
                "ShooterArena/Public/AI/Services",
                "ShooterArena/Public/AI/EQS",
                "ShooterArena/Public/AI/Decorators",

                "ShooterArena/Public/Menu",
                "ShooterArena/Public/Menu/UI",

                "ShooterArena/Public/Sound",
            }
        );

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
