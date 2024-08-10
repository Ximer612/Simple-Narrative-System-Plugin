// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Structs/SNS_S_Dialogue.h"
#include "Structs/SNS_S_SettingsData.h"
#include "SNS_NarrativeBlueprintFuncLib.h"
#include "SNS_Widget.h"
#include "SNS_DialogueWorldSubsystem.generated.h"

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

	float DialogueLineElapsedTime;
	float DialogueLineRemaningTime;

	TSubclassOf<USNS_Widget> SubtitlesWidgetClass; // static?
	TObjectPtr<USNS_Widget> SubtitlesWidget;

	TQueue<FSNS_S_Dialogue, EQueueMode::Mpsc> DialoguesToPlay;
	int32 CurrentDialogueLineIndex;
	FSNS_S_Dialogue CurrentDialogue;

	TObjectPtr<UAudioComponent> AudioComponent;

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

	void EnqueueDialogue(const struct FSNS_S_Dialogue* InDialogue);


private:
	void CreateSubtitlesWidget(const UWorld& InWorld);
	void CreateAudioComponent(const UWorld& InWorld);
	void PlayDialogue(bool& AllLinesEnded);
	void ManageDialogueEnd();
	void SendDialogue();
	//void LoadWidgetSettings(const FSNS_S_SettingsData& NewSettingsData);

	friend USNS_NarrativeBlueprintFuncLib;
};
