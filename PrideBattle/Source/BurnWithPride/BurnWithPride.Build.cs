// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BurnWithPride : ModuleRules
{
	public BurnWithPride(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "AIModule",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "UMG",
            "Slate",
            "NavigationSystem"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "AssetRegistry"
        });

        PublicIncludePaths.AddRange(new string[] {
			"BurnWithPride",
			"BurnWithPride/Variant_Combat/Interfaces",
			"BurnWithPride/Enemy",
            "BurnWithPride/GravityController",
            "BurnWithPride/PathFinding",
            "BurnWithPride/StateTree",
            "BurnWithPride/Player",
            "BurnWithPride/Counter",
            "BurnWithPride/Bullet",
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
