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
UCLASS(Abstract,Blueprintable, Config = NarrativeSystemSettings)
class SIMPLENARRATIVESYSTEM_API USNS_Widget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	//virtual void NativeConstruct() override;


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	struct FSNS_S_SettingsData SettingsData;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnReceivedDialogue(const FSNS_S_Speaker& Speaker, const FSNS_S_TimeStamp& TimeStamp, const bool bCanBeSkipped);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCurrentLineEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void SetStyleFromSettings();

	UFUNCTION(BlueprintImplementableEvent)
	void StopAllOtherDialogues();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCurrentDialogueEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAllDialoguesEnd();
};
