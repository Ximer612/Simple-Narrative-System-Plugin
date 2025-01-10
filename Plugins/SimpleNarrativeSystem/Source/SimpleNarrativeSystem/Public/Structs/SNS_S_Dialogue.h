// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SNS_S_TimeStamp.h"
#include "SNS_S_Dialogue.generated.h"

/**
 * 
 */
 
class USoundBase;
 
USTRUCT(Blueprintable)
struct SIMPLENARRATIVESYSTEM_API FSNS_S_Dialogue : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	TSoftObjectPtr<USoundBase> AudioClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (RequiredAssetDataTags = "RowStructure=/Script/SimpleNarrativeSystem.SNS_S_Speaker"), Category = "SimpleNarrativeSystem")
	TObjectPtr<UDataTable> SpeakersDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "CanBeSkipped?"), Category = "SimpleNarrativeSystem")
	bool bCanBeSkipped = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleNarrativeSystem")
	TArray<FSNS_S_TimeStamp> TimeStamps;

	/**
	 * Can be overridden by subclasses; Called on every row when the owning data table is modified
	 * Allows for custom fix-ups, parsing, etc for user changes
	 * This will be called in addition to OnPostDataImport when importing
	 *
	 * @param InDataTable					The data table that owns this row
	 * @param InRowName						The name of the row we're performing fix-up on
	 */
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
};
