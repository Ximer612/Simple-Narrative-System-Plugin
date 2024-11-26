// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Structs/SNS_S_Dialogue.h"
#include "Structs/SNS_S_SettingsData.h"
#include "SNS_NarrativeBlueprintFuncLib.h"
#include "SNS_Widget.h"
#include "SNS_DialogueWorldSubsystem.generated.h"

class FSNS_Dialogue
{
public:
	FSNS_Dialogue(const FName InDialogueRowName, const UDataTable* InDialoguesDataTable) : DialogueRowName(InDialogueRowName), DialoguesDataTable(InDialoguesDataTable)
	{

	}

	bool operator==(const FSNS_Dialogue& Other) const
	{
		return DialogueRowName == Other.DialogueRowName;
	}

private:
	const FName DialogueRowName;
	const UDataTable* DialoguesDataTable;

	friend USNS_DialogueWorldSubsystem;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueDelegate, FName);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllDialogueEnd);

/**
 * 
 */
UCLASS()
class SIMPLENARRATIVESYSTEM_API USNS_DialogueWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	USNS_DialogueWorldSubsystem();

	~USNS_DialogueWorldSubsystem();
	

private:

	// FTickableGameObject StatId
	TStatId StatId;

	bool bIsDisabled;
	bool bNoSpeakerDataTable;
	bool bIsTickEnabled;
	bool bIsPlayingAudio;
	bool bShouldAdjustAudioTiming;

	float DialogueLineElapsedTime;
	float DialogueLineRemaningTime;
	
	TObjectPtr<USNS_Widget> SubtitlesWidget;

	int32 CurrentDialogueLineIndex;

	FName CurrentDialogueRowName;

	FSNS_S_Dialogue* CurrentDialogue;

	TArray<FSNS_Dialogue> DialoguesToPlay;

	TObjectPtr<UAudioComponent> AudioComponent;

	FOnDialogueDelegate OnCurrentDialogueEndDelegate;

	FOnDialogueDelegate OnCurrentDialogueStartDelegate;

	FOnDialogueDelegate OnAllDialoguesEndDelegate;

public:
	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem implementation End

	// FTickableGameObject implementation Begin
	virtual void Tick(float DeltaTime);
	virtual TStatId GetStatId() const;
	// FTickableGameObject implementation End

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	void EnqueueDialogue(const FSNS_Dialogue&& InDialogue, const bool bStopAllOtherDialogues);

private:
	void CreateSubtitlesWidget(const UWorld& InWorld);
	void CreateAudioComponent(const UWorld& InWorld);
	void PlayDialogue(bool& AllLinesEnded);
	void ManageDialogueEnd(bool bShouldRemoveFirst = true);
	void SendDialogue();

	void SkipCurrentLine();

	friend USNS_NarrativeBlueprintFuncLib;
};
