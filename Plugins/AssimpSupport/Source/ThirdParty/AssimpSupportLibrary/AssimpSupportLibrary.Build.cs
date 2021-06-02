// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class AssimpSupportLibrary : ModuleRules
{
	public AssimpSupportLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;


		PublicIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(ModuleDirectory, "include"),
				// ... add public include paths required here ...
			}
		);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			// PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "assimp.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "assimp-vc142-mt.lib"));
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libassimp.a"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android","armeabi-v7a" ,"libassimp.so"));

			AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin",
				Path.Combine(ModuleDirectory, "Assimp_API.XML")));
		}
	}
}