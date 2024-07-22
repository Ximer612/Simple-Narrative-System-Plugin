// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Structs/SNS_S_Dialogue.h"
#include "SNS_WorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLENARRATIVESYSTEM_API USNS_WorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

private:
	int32 ZOrder = 612;

	TObjectPtr<UUserWidget> SubtitlesUI;

	float DialogueLineElapsedTime;
	float DialogueLineRemaningTime;

	TQueue<const FSNS_S_Dialogue*, EQueueMode::Mpsc> DialoguesToPlay;

	TObjectPtr<AActor> WidgetManager;


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

	void OnReceivedDialogue(const struct FSNS_S_Dialogue* InDialogue);

private:
	void CreateSubtitlesWidget(const UWorld& InWorld);
};
