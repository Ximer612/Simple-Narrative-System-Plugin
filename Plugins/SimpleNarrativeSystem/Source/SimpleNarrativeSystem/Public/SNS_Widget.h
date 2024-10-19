// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/SNS_S_SettingsData.h"
#include "Structs/SNS_S_Speaker.h"
#include "SNS_Widget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Config = NarrativeSystemSettings)
class SIMPLENARRATIVESYSTEM_API USNS_Widget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual bool Initialize() override;


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	struct FSNS_S_SettingsData SettingsData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RequiredAssetDataTags = "RowStructure=/Script/SimpleNarrativeSystem.SNS_S_Dialogue"))
	TSoftObjectPtr<UDataTable> SpeakersDataTable;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnReceivedDialogue(const FSNS_S_Speaker& Speaker, const FText& Text);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAllDialoguesEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCurrentDialogueEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCurrentLineEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void SetStyleFromSettings();

	UFUNCTION(BlueprintImplementableEvent)
	void StopAllOtherDialogues();
};
