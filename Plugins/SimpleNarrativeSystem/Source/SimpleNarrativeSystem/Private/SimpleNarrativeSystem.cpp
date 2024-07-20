// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleNarrativeSystem.h"
#include <AssetRegistry/AssetRegistryModule.h>
#include <ISettingsModule.h>
#include "SNS_CustomProjectSettings.h"

#define LOCTEXT_NAMESPACE "FSimpleNarrativeSystemModule"

void RegisterSettings()
{
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
}

void UnRegisterSettings()
{
	// unregister settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "SimpleNarrativeSystem");
	}
}

void FSimpleNarrativeSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	// 

	RegisterSettings();

	//UPackage* NewPackage = CreatePackage(TEXT("/Game/Logo/Textures/LogoPackage"));
	//FString FileName = FPackageName::LongPackageNameToFilename(NewPackage->GetPathName(), FPackageName::GetAssetPackageExtension());
	//UDataTable SpeakersDataTable = NewObject<UDataTable>(nullptr,"SpeakerDT",EObjectFlags::RF_Public);
	//UPackage::SavePackage(NewPackage, SpeakersDataTable, RF_Public | RF_Standalone, *FileName);
	//FAssetRegistryModule::AssetCreated(nullptr);
}



void FSimpleNarrativeSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UnRegisterSettings();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleNarrativeSystemModule, SimpleNarrativeSystem)