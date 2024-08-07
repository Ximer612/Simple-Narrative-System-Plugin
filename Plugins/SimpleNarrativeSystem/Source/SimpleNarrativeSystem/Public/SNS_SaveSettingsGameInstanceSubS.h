// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Structs/SNS_S_SettingsData.h"
#include "SNS_DialogueWorldSubsystem.h"
#include "SNS_NarrativeBlueprintFuncLib.h"
#include "SNS_SaveSettingsGameInstanceSubS.generated.h"
/**
 * 
 */
UCLASS(config = SettingsSaveGameTest)
class SIMPLENARRATIVESYSTEM_API USNS_SaveSettingsGameInstanceSubS : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

private:

	UPROPERTY(Config)
	FSNS_S_SettingsData SettingsData;

	friend USNS_DialogueWorldSubsystem;
	friend USNS_NarrativeBlueprintFuncLib;
};
