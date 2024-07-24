// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_DialogueWorldSubsystem.h"
#include "SNS_CustomProjectSettings.h"
#include "SNS_I_Subtitles.h"

#include "AudioDevice.h"

void USNS_DialogueWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Inizialize called on %s"), *GetWorld()->GetName());

	bIsTickEnabled = false;

}

void USNS_DialogueWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogTemp, Warning, TEXT("Deinitialize on %s"), *GetWorld()->GetName());


	//if (SubtitlesUI)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Destroy on %p"), SubtitlesUI);
	//	SubtitlesUI->Destruct();
	//}

}

void USNS_DialogueWorldSubsystem::Tick(float DeltaTime)
{
	if (bIsTickEnabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ticking!!!!"));
	}
}

TStatId USNS_DialogueWorldSubsystem::GetStatId() const
{
	return StatId;
}

void USNS_DialogueWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	if (InWorld.IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSubtitlesWidget in world on %s"), *InWorld.GetName());

		//IF SUBTITLES ENABLES?
		CreateSubtitlesWidget(InWorld);

		//INIT AUDIO COMPONENT
		FAudioDevice::FCreateComponentParams Params = FAudioDevice::FCreateComponentParams(InWorld.GetAudioDeviceRaw());
		AudioComponent = NewObject<UAudioComponent>((UClass*)Params.AudioComponentClass);

		AudioComponent->bAutoActivate = false;
		AudioComponent->AttenuationSettings = Params.AttenuationSettings;
		AudioComponent->ConcurrencySet = Params.ConcurrencySet;

		AudioComponent->SetVolumeMultiplier(1.f);
		AudioComponent->SetPitchMultiplier(1.f);
		AudioComponent->bAllowSpatialization = false;
		AudioComponent->bIsUISound = true;
		AudioComponent->bAutoDestroy = false;
		AudioComponent->bIgnoreForFlushing = false; // true or false???
		AudioComponent->bStopWhenOwnerDestroyed = false;

		AudioComponent->RegisterComponentWithWorld(&InWorld);
	}


}

void USNS_DialogueWorldSubsystem::EnqueueDialogue(const FSNS_S_Dialogue* InDialogue)
{
	bIsPlayingAudio = !DialoguesToPlay.IsEmpty();

	DialoguesToPlay.Enqueue(*InDialogue);

	if (!bIsPlayingAudio)
	{
		bool AllLinesEnded;
		PlayDialogue(AllLinesEnded);
		
	}
}

void USNS_DialogueWorldSubsystem::TestAudio(USoundBase* NewSound)
{
	AudioComponent->SetSound(NewSound);
	AudioComponent->Play();
}

void USNS_DialogueWorldSubsystem::CreateSubtitlesWidget(const UWorld& InWorld)
{
	TSubclassOf<UUserWidget> DialogueWidgetBlueprint = GetDefault<USNS_CustomProjectSettings>()->DialogueWidgetBlueprint;

	SubtitlesUI = CreateWidget(InWorld.GetFirstPlayerController(), DialogueWidgetBlueprint);

	if (SubtitlesUI)
	{
		SubtitlesUI->AddToViewport(ZOrder);
	}

	UE_LOG(LogTemp, Warning, TEXT("CreateWidget in world on %p"), SubtitlesUI);
}

void USNS_DialogueWorldSubsystem::PlayDialogue(bool& AllLinesEnded)
{
	if (DialoguesToPlay.IsEmpty())
	{
		AllLinesEnded = true;
		return;
	}

	DialogueLineElapsedTime = 0;

	FSNS_S_Dialogue DialogueToPlay;

	DialoguesToPlay.Dequeue(DialogueToPlay);

	DialogueToPlay.AudioClip.LoadSynchronous();

	AudioComponent->SetSound(DialogueToPlay.AudioClip.Get());
	AudioComponent->Play(0.f);

	bIsTickEnabled = true;

	int test_index = 0;

	if (SubtitlesUI->Implements<USNS_I_Subtitles>())
	{
		ISNS_I_Subtitles* SubtitlesUIInterface = Cast<ISNS_I_Subtitles>(SubtitlesUI);

		TSoftObjectPtr<UDataTable> SpeakersDataTable = GetDefault<USNS_CustomProjectSettings>()->SpeakersDataTable;
		FSNS_S_Speaker* Speaker = SpeakersDataTable->FindRow<FSNS_S_Speaker>(DialogueToPlay.TimeStamps[test_index].Speaker.RowName, "", true);
		SubtitlesUIInterface->Execute_OnReceivedDialogue(SubtitlesUI, *Speaker, DialogueToPlay.TimeStamps[test_index].SubtitleText);
	}
}
