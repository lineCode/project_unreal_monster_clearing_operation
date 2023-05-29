// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MonsterCO : ModuleRules
{
	public MonsterCO(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GameplayTasks" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks", "ModularGameplay", "GameFeatures", "AnimGraphRuntime" });

        PublicIncludePaths.AddRange(new string[] { "MonsterCO" });
    }
}
