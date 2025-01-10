// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Structs/SNS_S_Dialogue.h"
#include "Structs/SNS_S_SettingsData.h"
#include "SNS_NarrativeBlueprintFuncLib.h"
#include "SNS_Manager.h"
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

struct FDialogueLambda
{
	FDelegateHandle DelegateHandle;
	bool bRepeatable;
};

struct FDialogueEventsLambdas
{
	TArray<FDialogueLambda> OnStart;
	TArray<FDialogueLambda> OnEnd;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueDelegate, FName);

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

	bool bTESTVARIABLEREMOVEME = true;

	// FTickableGameObject StatId
	TStatId StatId;

	bool bIsDisabled;
	bool bNoSpeakerDataTable;
	bool bIsTickEnabled;
	bool bIsPlayingAudio;
	bool bShouldAdjustAudioTiming;

	float DialogueLineElapsedTime;
	float DialogueLineRemaningTime;
	
	TObjectPtr<ASNS_Manager> InGameManager;
	TArray<FSNS_Dialogue> DialoguesToPlay;
	TMap<FName, FDialogueEventsLambdas> PerDialogueLambdas;
	TArray<FDialogueLambda> PerDialogueLambdasOnAllEnd;

	int32 CurrentDialogueLineIndex;
	FName CurrentDialogueRowName;
	FSNS_S_Dialogue* CurrentDialogue;

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
	void PlayDialogue(bool& AllLinesEnded);
	void ManageDialogueEnd(bool bShouldRemoveFirst = true);
	void SendDialogueToWidget();

	void SkipCurrentLine();

	void CheckDialogueMapContainsRowName(const FName& DialogueRowName);

	void AddOnCurrentDialogueEnd(const FName& DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueEnd);
	void AddOnCurrentDialogueStart(const FName& DialogueRowName, const bool bRepeatable, const FRegisteredDelegate& OnDialogueStart);
	void AddOnAllCurrentDialogueEnd(const bool bRepeatable, const FRegisteredDelegate& OnAllDialogueEnd);
	void ClearTMap();

	void CallDialogueDelegate(FOnDialogueDelegate& InDialogueDelegate, const FName& InDialogueRowName, const bool bOnStart);
	
	friend USNS_NarrativeBlueprintFuncLib;
};
