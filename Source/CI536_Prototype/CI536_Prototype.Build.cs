// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CI536_Prototype : ModuleRules
{
	public CI536_Prototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
