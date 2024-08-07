// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#define GET_WORLD_FROM_CONTEXT(World,WorldContextObject) World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

#include "SNS_NarrativeBlueprintFuncLib.h"
#include "Structs/SNS_S_Dialogue.h"
#include "SNS_DialogueWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SNS_SettingsSaveGame.h"

#include "SNS_SaveSettingsGameInstanceSubS.h"

#define LOCTEXT_NAMESPACE "SNS_NameSpace"

void USNS_NarrativeBlueprintFuncLib::EnqueueDialogue(UObject* WorldContextObject, const FName DialogueRowName, const UDataTable* DialoguesDataTable)
{
	if (DialogueRowName == TEXT("") || DialogueRowName == TEXT("None"))
	{
		FMessageLog("PIE").Error(LOCTEXT("InvalidRow", "Dialogue row name cannot be None/Null!"));
		return;
	}

	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	FSNS_S_Dialogue* Dialogue = DialoguesDataTable->FindRow<FSNS_S_Dialogue>(DialogueRowName, "", true);

	if (!Dialogue)
	{
		FMessageLog("PIE").Error(FText::Format(LOCTEXT("NotFoundRow", "Dialogue row name '{0}' cannot be found!"), FText::FromName(DialogueRowName)));
		return;
	}

	if (CurrentWorld)
	{
		USNS_DialogueWorldSubsystem* NarrativeSubSystem = CurrentWorld->GetSubsystem<USNS_DialogueWorldSubsystem>();
		NarrativeSubSystem->EnqueueDialogue(Dialogue);
	}
}

void USNS_NarrativeBlueprintFuncLib::SaveSettings(UObject* WorldContextObject,const FSNS_S_SettingsData& SettingsData)
{
	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	UGameInstance* GameInstance = CurrentWorld->GetGameInstance();

	if (!GameInstance)
	{
		return;
	}

	USNS_SaveSettingsGameInstanceSubS* SettingsGameInstance = GameInstance->GetSubsystem<USNS_SaveSettingsGameInstanceSubS>();

	if (!SettingsGameInstance)
	{
		return;
	}

	SettingsGameInstance->SettingsData = SettingsData;
	SettingsGameInstance->SaveConfig(CPF_Config, *SettingsGameInstance->GetDefaultConfigFilename());

	return;
}

const void USNS_NarrativeBlueprintFuncLib::LoadSettings(UObject* WorldContextObject, FSNS_S_SettingsData& SettingsData)
{
	UWorld* CurrentWorld;

	GET_WORLD_FROM_CONTEXT(CurrentWorld, WorldContextObject);

	UGameInstance* GameInstance = CurrentWorld->GetGameInstance();

	if (!GameInstance)
	{
		return;
	}

	USNS_SaveSettingsGameInstanceSubS* SettingsGameInstance = GameInstance->GetSubsystem<USNS_SaveSettingsGameInstanceSubS>();

	if (!SettingsGameInstance)
	{
		return;
	}

	SettingsData = SettingsGameInstance->SettingsData;

	return;
}
