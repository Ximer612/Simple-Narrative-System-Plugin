// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_WorldSubsystem.h"
#include "SNS_CustomProjectSettings.h"

#include "SNS_I_Subtitles.h"

void USNS_WorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Inizialize called on %s"), *GetWorld()->GetName());

}

void USNS_WorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Warning, TEXT("Deinitialize on %s"), *GetWorld()->GetName());


	//if (SubtitlesUI)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Destroy on %p"), SubtitlesUI);
	//	SubtitlesUI->Destruct();
	//}

}

void USNS_WorldSubsystem::Tick(float DeltaTime)
{

}

TStatId USNS_WorldSubsystem::GetStatId() const
{
	return TStatId();
}

void USNS_WorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	if (InWorld.IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSubtitlesWidget in world on %s"), *InWorld.GetName());

		//IF SUBTITLES ENABLES?
		CreateSubtitlesWidget(InWorld);
	}


}

void USNS_WorldSubsystem::OnReceivedDialogue(const FSNS_S_Dialogue* InDialogue)
{
	DialoguesToPlay.Enqueue(InDialogue);

	int test_index = 0;

	if (SubtitlesUI->Implements<USNS_I_Subtitles>())
	{
		ISNS_I_Subtitles* SubtitlesUIInterface = Cast<ISNS_I_Subtitles>(SubtitlesUI);

		TSoftObjectPtr<UDataTable> SpeakersDataTable = GetDefault<USNS_CustomProjectSettings>()->SpeakersDataTable;
		FSNS_S_Speaker* Speaker = SpeakersDataTable->FindRow<FSNS_S_Speaker>(InDialogue->TimeStamps[test_index].Speaker.RowName, "", true);
		SubtitlesUIInterface->Execute_OnReceivedDialogue(SubtitlesUI, *Speaker, InDialogue->TimeStamps[test_index].SubtitleText);
	}
}

void USNS_WorldSubsystem::CreateSubtitlesWidget(const UWorld& InWorld)
{
	TSubclassOf<UUserWidget> DialogueWidgetBlueprint = GetDefault<USNS_CustomProjectSettings>()->DialogueWidgetBlueprint;

	SubtitlesUI = CreateWidget(InWorld.GetFirstPlayerController(), DialogueWidgetBlueprint);

	if (SubtitlesUI)
	{
		SubtitlesUI->AddToViewport(ZOrder);
	}

	UE_LOG(LogTemp, Warning, TEXT("CreateWidget in world on %p"), SubtitlesUI);
}
