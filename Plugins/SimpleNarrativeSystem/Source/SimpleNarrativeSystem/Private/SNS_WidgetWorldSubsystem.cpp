// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_WidgetWorldSubsystem.h"

void USNS_WidgetWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Inizialize called on %s"), *GetWorld()->GetName());

	if (GetWorld()->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnActor in world on %s"), *GetWorld()->GetName());

		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		SpawnParams.Name = "Subtitle Narrative System Manager";
		//NarrativeSystem = GetWorld()->SpawnActor<ASNS_SubtitleManager>(SpawnParams);
	}
}

void USNS_WidgetWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Warning, TEXT("Deinitialize on %s"), *GetWorld()->GetName());

	//if (NarrativeSystem)
	//{
	//	NarrativeSystem->Destroy();
	//UE_LOG(LogTemp, Warning, TEXT("Destroy on %s"), *GetWorld()->GetName());
	//}
}

void USNS_WidgetWorldSubsystem::Tick(float DeltaTime)
{

}

TStatId USNS_WidgetWorldSubsystem::GetStatId() const
{
	return TStatId();
}
