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
	bIsPlayingAudio = false;
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
		//UE_LOG(LogTemp, Warning, TEXT("Ticking!!!!"));

		DialogueLineElapsedTime += DeltaTime;
		DialogueLineRemaningTime -= DeltaTime;

		//if remaning time is over
		if (DialogueLineRemaningTime < 0)
		{
			//if there aren't other timestamps
			if (CurrentDialogueLineIndex >= CurrentDialogue.TimeStamps.Num())
			{
				ManageDialogueEnd(); //better name/organization
				return;
			}

			//SEND DIALOGUYE
			SendDialogue();

			CurrentDialogueLineIndex++;
		}

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

		//IF SUBTITLES ENABLED?
		CreateSubtitlesWidget(InWorld);

		//IF AUDIO ENABLED?
		CreateAudioComponent(InWorld);
	}
}

void USNS_DialogueWorldSubsystem::EnqueueDialogue(const FSNS_S_Dialogue* InDialogue)
{
	DialoguesToPlay.Enqueue(*InDialogue);

	if (!bIsPlayingAudio)
	{
		bool AllLinesEnded;
		PlayDialogue(AllLinesEnded);
		
	}
}

void USNS_DialogueWorldSubsystem::CreateSubtitlesWidget(const UWorld& InWorld)
{
	TSubclassOf<UUserWidget> DialogueWidgetBlueprint = GetDefault<USNS_CustomProjectSettings>()->DialogueWidgetBlueprint;

	SubtitlesUI = CreateWidget(InWorld.GetFirstPlayerController(), DialogueWidgetBlueprint);

	if (!SubtitlesUI->Implements<USNS_I_Subtitles>())
	{
		UE_LOG(LogTemp, Error, TEXT("SubtitlesUI dosen't implements Subtitles Interface!"));
		return;
	}
	
	SubtitlesUIInterface = Cast<ISNS_I_Subtitles>(SubtitlesUI);

	if (SubtitlesUI)
	{
		SubtitlesUI->AddToViewport(ZOrder);
	}

	UE_LOG(LogTemp, Log, TEXT("CreateWidget in world on %p"), SubtitlesUI);
}

void USNS_DialogueWorldSubsystem::CreateAudioComponent(const UWorld& InWorld)
{
	//INIT AUDIO COMPONENT
	FAudioDevice::FCreateComponentParams Params = FAudioDevice::FCreateComponentParams(InWorld.GetAudioDeviceRaw());
	AudioComponent = NewObject<UAudioComponent>((UClass*)Params.AudioComponentClass, "AudioComponent", RF_Standalone);

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

	AudioComponent->RegisterComponentWithWorld(InWorld.GetWorld());
}

void USNS_DialogueWorldSubsystem::PlayDialogue(bool& AllLinesEnded)
{
	if (DialoguesToPlay.IsEmpty())
	{
		AllLinesEnded = true;
		return;
	}

	AllLinesEnded = false;

	DialogueLineElapsedTime = 0;

	DialoguesToPlay.Dequeue(CurrentDialogue);

	//TODO:check if it's only text

	AudioComponent->SetSound(CurrentDialogue.AudioClip.LoadSynchronous());
	AudioComponent->Play(0.f);

	CurrentDialogueLineIndex = 0;

	bIsTickEnabled = true;
	bIsPlayingAudio = true;
}

void USNS_DialogueWorldSubsystem::ManageDialogueEnd()
{
	if (AudioComponent->Sound != nullptr)
	{
		AudioComponent->Stop();
	}

	bIsPlayingAudio = false;

	//IF NO MORE DIALOGUES TO PLAY
	if (DialoguesToPlay.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("End dialogue!!!!"));
		//CALL ON END ALL DIALOGUES
	}
	//IF THERE ARE OTHER DIALOGUES

	bool AllDialoguesEnded;
	PlayDialogue(AllDialoguesEnded);

	//ON END ALL CURRENT DIALOGUES LINES (animation to remove subtitle)

	bIsTickEnabled = !AllDialoguesEnded;
}

void USNS_DialogueWorldSubsystem::SendDialogue()
{
	TSoftObjectPtr<UDataTable> SpeakersDataTable = GetDefault<USNS_CustomProjectSettings>()->SpeakersDataTable;
	FSNS_S_Speaker* Speaker = SpeakersDataTable->FindRow<FSNS_S_Speaker>(CurrentDialogue.TimeStamps[CurrentDialogueLineIndex].Speaker.RowName, "", true);

	if (!Speaker)
	{
		UE_LOG(LogTemp, Error, TEXT("SPEAKER %s NOT FOUND!!!!"), *CurrentDialogue.TimeStamps[CurrentDialogueLineIndex].Speaker.RowName.ToString());
		return;
	}

	DialogueLineRemaningTime += CurrentDialogue.TimeStamps[CurrentDialogueLineIndex].TimeStamp - DialogueLineElapsedTime;

	SubtitlesUIInterface->Execute_OnReceivedDialogue(SubtitlesUI, *Speaker, CurrentDialogue.TimeStamps[CurrentDialogueLineIndex].SubtitleText);
}