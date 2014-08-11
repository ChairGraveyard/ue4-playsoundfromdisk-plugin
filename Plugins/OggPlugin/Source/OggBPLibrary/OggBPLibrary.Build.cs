// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OggBPLibrary : ModuleRules
{
	public OggBPLibrary(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(
			new string[] { 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore",
				"RHI"
			}
		);
		//Private Paths
        PrivateIncludePaths.AddRange(new string[] { 
			"OggBPLibrary/Public",
			"OggBPLibrary/Private"
		});
	}
}
