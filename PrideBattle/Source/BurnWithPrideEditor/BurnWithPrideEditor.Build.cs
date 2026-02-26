using UnrealBuildTool;

public class BurnWithPrideEditor : ModuleRules
{
    public BurnWithPrideEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AssetRegistry",
            "UnrealEd",   // Editor-only
            "AssetTools",
            "ToolMenus",
            "Projects"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            // •K—v‚È‚ç‚³‚ç‚É’Ç‰Á
        });
    }
}