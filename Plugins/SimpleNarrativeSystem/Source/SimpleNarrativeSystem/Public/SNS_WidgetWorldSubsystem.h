// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "SNS_WidgetWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLENARRATIVESYSTEM_API USNS_WidgetWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

private:
	int32 ZOrder = 612;

	float DialogueLineElapsedTime;
	float DialogueLineRemaningTime;

	TQueue<FName, EQueueMode::Mpsc> AudiosToPlay;

	TSubclassOf<UUserWidget> SubtitlesWidgetTemplate;

	TObjectPtr<UUserWidget> NarrativeSystemInstance;


public:
	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem implementation End

	// FTickableGameObject implementation Begin
	virtual void Tick(float DeltaTime);
	virtual TStatId GetStatId() const;
	// FTickableGameObject implementation End
};
