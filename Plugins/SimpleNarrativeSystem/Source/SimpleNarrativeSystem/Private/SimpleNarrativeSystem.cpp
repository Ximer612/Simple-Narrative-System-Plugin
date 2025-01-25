// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#include "SimpleNarrativeSystem.h"
#include "SNS_CustomProjectSettings.h"

#if WITH_EDITOR
	#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "FSimpleNarrativeSystemModule"

void FSimpleNarrativeSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterSettings();
}

void FSimpleNarrativeSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UnRegisterSettings();
}

void FSimpleNarrativeSystemModule::RegisterSettings()
{
#if WITH_EDITOR
// register settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "SimpleNarrativeSystem",
			LOCTEXT("SimpleNarrativeSystemSettingsName", "Simple Narrative System"),
			LOCTEXT("SimpleNarrativeSystemSettingsDescription", "Configure the Simple Narrative System plugin."),
			GetMutableDefault<USNS_CustomProjectSettings>()
		);
	}
#endif
}

void FSimpleNarrativeSystemModule::UnRegisterSettings()
{
#if WITH_EDITOR
	// unregister settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "SimpleNarrativeSystem");
	}
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleNarrativeSystemModule, SimpleNarrativeSystem)