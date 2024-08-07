// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SNS_S_TimeStamp.h"
#include "SNS_S_Dialogue.generated.h"

/**
 * 
 */
 
class USoundWave;
 
USTRUCT(Blueprintable)
struct SIMPLENARRATIVESYSTEM_API FSNS_S_Dialogue : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USoundWave> AudioClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSNS_S_TimeStamp> TimeStamps;
};
