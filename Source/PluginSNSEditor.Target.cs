// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PluginSNSEditorTarget : TargetRules
{
	public PluginSNSEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("PluginSNS");
		
		// Game editor
        ExtraModuleNames.AddRange(
            new string[]
            {
                "SimpleNarrativeSystemEditor"
            });
	}
}
