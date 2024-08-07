// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_SaveSettingsGameInstanceSubS.h"
#include "SNS_SettingsSaveGame.h"
#if WITH_EDITOR
#include "SNS_CustomProjectSettings.h"
#endif

void USNS_SaveSettingsGameInstanceSubS::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

#if WITH_EDITOR

	const USNS_CustomProjectSettings* CustomProjectSettings = GetDefault<USNS_CustomProjectSettings>();

	SettingsData.bSubtitlesEnabled = CustomProjectSettings->bSubtitlesEnabled;
	SettingsData.ZOrder = CustomProjectSettings->ZOrder;
	SettingsData.SpeakersDataTable = CustomProjectSettings->SpeakersDataTable;
	SettingsData.DialogueWidgetBlueprint = CustomProjectSettings->DialogueWidgetBlueprint;

	UE_LOG(LogTemp, Warning, TEXT("I'm bein initialized !!"));

	SaveConfig(CPF_Config, *GetDefaultConfigFilename());

	UE_LOG(LogTemp, Warning, TEXT("Test var sub size %d"), SettingsData.SubtitlesSize);

#endif

}

void USNS_SaveSettingsGameInstanceSubS::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Warning, TEXT("I'm bein deinitialized !!"));
}
