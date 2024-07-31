// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_DialogueWorldSubsystem.h"
#include "SNS_CustomProjectSettings.h"
#include "SNS_I_Subtitles.h"
#include "AudioDevice.h"

#define LOCTEXT_NAMESPACE "SNS_NameSpace"
#define GET_SETTINGS GetDefault<USNS_CustomProjectSettings>()

void USNS_DialogueWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	bIsTickEnabled = false;
	bIsPlayingAudio = false;
}

void USNS_DialogueWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if(SubtitlesUI)
		SubtitlesUI->Destruct();
	if(WidgetManager)
		WidgetManager->Destroy();
	if (AudioComponent)
		AudioComponent->DestroyComponent();

}

void USNS_DialogueWorldSubsystem::Tick(float DeltaTime)
{
	if (bIsTickEnabled)
	{
		DialogueLineElapsedTime += DeltaTime;
		DialogueLineRemaningTime -= DeltaTime;

		//if remaning time is over
		if (DialogueLineRemaningTime < 0)
		{

			if (CurrentDialogueLineIndex != 0 && SubtitlesUI)
			{
				SubtitlesUIInterface->Execute_OnCurrentLineEnd(SubtitlesUI);
			}
			//if there aren't other timestamps
			if (CurrentDialogueLineIndex >= CurrentDialogue.TimeStamps.Num())
			{
				ManageDialogueEnd(); //check if ended all dialogues or just current
				return;
			}

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
		//IF SUBTITLES ENABLED
		if (GET_SETTINGS->bSubtitlesEnabled)
		{
			CreateSubtitlesWidget(InWorld);
		}

		//IF AUDIO ENABLED?
		CreateAudioComponent(InWorld);

		if (GET_SETTINGS->SpeakersDataTable == nullptr)
		{
			FMessageLog("PIE").Error(LOCTEXT("NullSpeakersDataTable", "Please set a data table for speakers in the plugin's project settings!"));
			bNoSpeakerDataTable = true;
		}
	}
}

void USNS_DialogueWorldSubsystem::EnqueueDialogue(const FSNS_S_Dialogue* InDialogue)
{
	if (bNoSpeakerDataTable)
	{
		return;
	}

	DialoguesToPlay.Enqueue(*InDialogue);

	if (!bIsPlayingAudio)
	{
		bool AllLinesEnded;
		PlayDialogue(AllLinesEnded);		
	}
}

void USNS_DialogueWorldSubsystem::CreateSubtitlesWidget(const UWorld& InWorld)
{
	TSubclassOf<UUserWidget> DialogueWidgetBlueprint = GET_SETTINGS->DialogueWidgetBlueprint;

	if (DialogueWidgetBlueprint == nullptr)
	{
		FMessageLog("PIE").Error(LOCTEXT("DialogueWidgetNull", "Please set a widget for subtitles or set 'subtitles enabled' to false in the plugin's project settings!"));
		return;
	}

	SubtitlesUI = CreateWidget(InWorld.GetFirstPlayerController(), DialogueWidgetBlueprint);

	if (!SubtitlesUI->Implements<USNS_I_Subtitles>())
	{
		FMessageLog("PIE").Error(LOCTEXT("DialogueWidgetNoInterface", "Selected widget for subtitles dosen't implements SNS_I_Subtitles!"));
		return;
	}
	
	SubtitlesUIInterface = Cast<ISNS_I_Subtitles>(SubtitlesUI);

	if (SubtitlesUI)
	{
		SubtitlesUI->AddToViewport(GET_SETTINGS->ZOrder);
	}
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

	if (ensure(InWorld.GetWorld()))
	{
		AudioComponent->RegisterComponentWithWorld(InWorld.GetWorld());
	}
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

	CurrentDialogue.AudioClip.LoadSynchronous();

	if (CurrentDialogue.AudioClip)
	{
		AudioComponent->SetSound(CurrentDialogue.AudioClip.LoadSynchronous());
		AudioComponent->Play(0.f);
	}

	CurrentDialogueLineIndex = 0;

	bIsTickEnabled = true;
	bIsPlayingAudio = true;
}

void USNS_DialogueWorldSubsystem::ManageDialogueEnd()
{
	//stops the audio if the last timestamp dosen't match with it's end
	if (AudioComponent->Sound != nullptr)
	{
		AudioComponent->Stop();
	}

	bIsPlayingAudio = false;

	if (SubtitlesUI)
	{
		//call ON END ALL CURRENT DIALOGUES LINES (animation to remove subtitle)
		SubtitlesUIInterface->Execute_OnCurrentDialogueEnd(SubtitlesUI);
	}

	//if there aren't other dialogues in queue to play
	if (DialoguesToPlay.IsEmpty())
	{
		bIsTickEnabled = false;
		//CALL ON END ALL DIALOGUES
		if (SubtitlesUI)
		{
			SubtitlesUIInterface->Execute_OnAllDialoguesEnd(SubtitlesUI);
		}
	}
	else
	{
		//play next queue dialogue
		bool AllDialoguesEnded;
		PlayDialogue(AllDialoguesEnded);
		bIsTickEnabled = true;
	}
}

void USNS_DialogueWorldSubsystem::SendDialogue()
{
	TSoftObjectPtr<UDataTable> SpeakersDataTable = GET_SETTINGS->SpeakersDataTable;
	FSNS_S_Speaker* Speaker = SpeakersDataTable->FindRow<FSNS_S_Speaker>(CurrentDialogue.TimeStamps[CurrentDialogueLineIndex].Speaker.RowName, "", true);

	if (!Speaker)
	{
		FMessageLog("LogSNS").Error(LOCTEXT("SpeakerNotFound", "A speaker was not found! (read console for more info)"));
		UE_LOG(LogTemp, Error, TEXT("SPEAKER %s NOT FOUND!!!!"), *CurrentDialogue.TimeStamps[CurrentDialogueLineIndex].Speaker.RowName.ToString());
		return;
	}

	DialogueLineRemaningTime += CurrentDialogue.TimeStamps[CurrentDialogueLineIndex].TimeStamp - DialogueLineElapsedTime;

	if (SubtitlesUI)
	{
		SubtitlesUIInterface->Execute_OnReceivedDialogue(SubtitlesUI, *Speaker, CurrentDialogue.TimeStamps[CurrentDialogueLineIndex].SubtitleText);
	}
}
