// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#include "SNS_SettingsGameInstanceSS.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogSNS);

void USNS_SettingsGameInstanceSS::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadSettings();

}

void USNS_SettingsGameInstanceSS::Deinitialize()
{
	Super::Deinitialize();
}

void USNS_SettingsGameInstanceSS::SaveSettings(const FSNS_S_SettingsData& NewSettings)
{
	if (!SettingsSaveGame)
	{
		USaveGame* NewSettingsSaveGame = UGameplayStatics::CreateSaveGameObject(SettingsSaveGame->StaticClass());

		if (!NewSettingsSaveGame)
		{
			return;
		}

		SettingsSaveGame = Cast<USNS_SettingsSaveGame>(NewSettingsSaveGame);

		if (!SettingsSaveGame)
		{
			return;
		}
	}

	SettingsSaveGame->SettingsData = NewSettings;

	UGameplayStatics::SaveGameToSlot(SettingsSaveGame, SettingsSlotName, 0);

	OnSaveSettings.ExecuteIfBound();
}

FSNS_S_SettingsData& USNS_SettingsGameInstanceSS::GetSettings()
{
	return SettingsSaveGame->SettingsData;
}

bool USNS_SettingsGameInstanceSS::LoadSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(SettingsSlotName, 0))
	{
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SettingsSlotName, 0);

		if (!SaveGame)
		{
			//corrupted
			return false;
		}

		SettingsSaveGame = Cast<USNS_SettingsSaveGame>(SaveGame);

		if (!SettingsSaveGame)
		{
			//corrupted
			return false;
		}
		return true;
	}

	return false;
}

bool USNS_SettingsGameInstanceSS::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (FParse::Command(&Cmd, TEXT("deletesns")))
	{
		DeleteSavedSettings();
		return true;
	}
	return false;
}

void USNS_SettingsGameInstanceSS::DeleteSavedSettings(bool bOnlyInEditor)
{
	if (!bOnlyInEditor)
	{
		UGameplayStatics::DeleteGameInSlot(SettingsSlotName, 0);
	}

#if WITH_EDITOR
	UGameplayStatics::DeleteGameInSlot(SettingsSlotName, 0);
#endif
	UE_LOG(LogSNS, Log, TEXT("Deleted saved simple narrative system widget settings!") );
}
