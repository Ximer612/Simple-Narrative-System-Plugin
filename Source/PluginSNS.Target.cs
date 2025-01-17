// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PluginSNSTarget : TargetRules
{
	public PluginSNSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("PluginSNS");
		
		if (bBuildEditor)
        {
            ExtraModuleNames.AddRange( 
                new string[]
                {
                    "SimpleNarrativeSystemEditor"
                });
        }
	}
}
