// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SNS_S_TimeStamp.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct SIMPLENARRATIVESYSTEM_API FSNS_S_TimeStamp
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SpeakerDataTableName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeStamp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText SubtitleText;
};
