// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Structs/SNS_S_SettingsData.h"
#include "SNS_DialogueWorldSubsystem.h"
#include "SNS_SettingsSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SNS_SettingsGameInstanceSS.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSNS, Log, All);
DECLARE_DELEGATE(FOnSaveSettings);

/**
 * 
 */
UCLASS()
class SIMPLENARRATIVESYSTEM_API USNS_SettingsGameInstanceSS : public UGameInstanceSubsystem, public FSelfRegisteringExec
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "SimpleNarrativeSystem")
	void SaveSettings(const FSNS_S_SettingsData& NewSettings);

	UFUNCTION(BlueprintCallable, Category = "SimpleNarrativeSystem")
	FSNS_S_SettingsData& GetSettings();

	UFUNCTION(BlueprintCallable, Category = "SimpleNarrativeSystem")
	bool LoadSettings();
	UFUNCTION(BlueprintCallable, Category = "SimpleNarrativeSystem")
	void DeleteSavedSettings(UPARAM(DisplayName ="OnlyInEditor?") bool bOnlyInEditor = false);

	FOnSaveSettings OnSaveSettings;

	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

private:
	UPROPERTY()
	USNS_SettingsSaveGame* SettingsSaveGame;

	const FString SettingsSlotName = "SNS_Settings";
};
