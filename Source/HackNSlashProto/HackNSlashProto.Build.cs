// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HackNSlashProto : ModuleRules
{
	public HackNSlashProto(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "UMG", "ModelViewViewModel"});
        
        if (Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
	        PrivateDependencyModuleNames.Add("ImGui");
        }
    }
}
