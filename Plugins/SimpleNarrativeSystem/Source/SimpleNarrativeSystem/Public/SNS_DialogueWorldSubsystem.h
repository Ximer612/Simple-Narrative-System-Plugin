// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Structs/SNS_S_Dialogue.h"
#include "SNS_DialogueWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLENARRATIVESYSTEM_API USNS_DialogueWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

private:
	int32 ZOrder = 612;

	bool bIsTickEnabled;
	bool bIsPlayingAudio;

	float DialogueLineElapsedTime;
	float DialogueLineRemaningTime;

	TStatId StatId;
	
	TObjectPtr<UUserWidget> SubtitlesUI;

	TQueue<FSNS_S_Dialogue, EQueueMode::Mpsc> DialoguesToPlay;

	TObjectPtr<AActor> WidgetManager;

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

	void TestAudio(USoundBase* NewSound);

private:
	void CreateSubtitlesWidget(const UWorld& InWorld);

	void PlayDialogue(bool& AllLinesEnded);

};
