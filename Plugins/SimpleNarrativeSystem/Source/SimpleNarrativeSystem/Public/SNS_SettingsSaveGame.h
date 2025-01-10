// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Structs/SNS_S_SettingsData.h"
#include "GameFramework/SaveGame.h"
#include "SNS_SettingsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class USNS_SettingsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleNarrativeSystem")
	FSNS_S_SettingsData SettingsData;
	
};
