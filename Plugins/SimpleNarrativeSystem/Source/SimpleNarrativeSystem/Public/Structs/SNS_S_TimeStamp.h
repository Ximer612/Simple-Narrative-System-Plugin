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
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "/Script/SimpleNarrativeSystem.SNS_S_Speaker"))
	FDataTableRowHandle Speaker;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeStamp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText SubtitleText;

	FSNS_S_TimeStamp();
	~FSNS_S_TimeStamp();

};
