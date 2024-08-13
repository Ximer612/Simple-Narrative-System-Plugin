// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Structs/SNS_S_SettingsData.h"
#include "SNS_DialogueWorldSubsystem.h"
#include "SNS_SettingsSaveGame.h"
#include "SNS_SettingsGameInstanceSS.generated.h"

DECLARE_DELEGATE(FOnSaveSettings);

/**
 * 
 */
UCLASS()
class SIMPLENARRATIVESYSTEM_API USNS_SettingsGameInstanceSS : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void SaveSettings(const FSNS_S_SettingsData& NewSettings);

	UFUNCTION(BlueprintCallable)
	FSNS_S_SettingsData& GetSettings();

	UFUNCTION(BlueprintCallable)
	bool LoadSettings();

	FOnSaveSettings OnSaveSettings;

private:
	UPROPERTY()
	USNS_SettingsSaveGame* SettingsSaveGame;

	const FString SettingsSlotName = "SNS_Settings";
};
