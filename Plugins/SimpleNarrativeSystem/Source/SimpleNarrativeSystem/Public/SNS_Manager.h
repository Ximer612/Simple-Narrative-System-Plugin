// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AudioDevice.h"
#include "SNS_Widget.h"
#include "GameFramework/Actor.h"
#include "SNS_Manager.generated.h"

UCLASS(Blueprintable)
class SIMPLENARRATIVESYSTEM_API ASNS_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASNS_Manager();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="SimpleNarrativeSystem")
	TObjectPtr<USNS_Widget> SubtitlesWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleNarrativeSystem")
	TObjectPtr<UAudioComponent> AudioComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
