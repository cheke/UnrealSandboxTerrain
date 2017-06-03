// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealSandboxTerrain : ModuleRules
{
	public UnrealSandboxTerrain(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"UnrealSandboxTerrain/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"UnrealSandboxTerrain/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "RenderCore",
                "ShaderCore",
                "RHI",
                "Json",
                "JsonUtilities",
                "PolygonalMapGenerator",
                "GameplayTags"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "RenderCore",
                "ShaderCore",
                "RHI",
                "Json",
                "JsonUtilities",
		        "PolygonalMapGenerator",
                "GameplayTags"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
