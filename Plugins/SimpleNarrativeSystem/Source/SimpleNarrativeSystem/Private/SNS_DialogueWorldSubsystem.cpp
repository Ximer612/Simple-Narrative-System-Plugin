// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#include "SNS_DialogueWorldSubsystem.h"
#include "SNS_I_Subtitles.h"
#include "AudioDevice.h"

#include "SNS_NarrativeBlueprintFuncLib.h"


#define LOCTEXT_NAMESPACE "SNS_NameSpace"

USNS_DialogueWorldSubsystem::USNS_DialogueWorldSubsystem()
{
	//TODO: Remove this hard coded reference
	static ConstructorHelpers::FClassFinder<UUserWidget> AssetFile(TEXT("/SimpleNarrativeSystem/UserInterface/WBP_Subtitles.WBP_Subtitles_C"));

	if (AssetFile.Class != nullptr)
	{
		SubtitlesWidgetClass = AssetFile.Class;
	}
}

USNS_DialogueWorldSubsystem::~USNS_DialogueWorldSubsystem()
{
}

void USNS_DialogueWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	bIsTickEnabled = false;
	bIsPlayingAudio = false;
}

void USNS_DialogueWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if(SubtitlesWidget)
		SubtitlesWidget->Destruct();
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
			//AudioComponent->SetPaused(true);

			//if it's not the first subtitle line
			if (CurrentDialogueLineIndex != 0 && SubtitlesWidget)
			{
				SubtitlesWidget->OnCurrentLineEnd();
			}
			//if there aren't other timestamps
			if (CurrentDialogueLineIndex >= CurrentDialogue->TimeStamps.Num())
			{
				ManageDialogueEnd(); //check if ended all dialogues or just current
				return;
			}

			//if (CurrentDialogueLineIndex == 0)
			//{

			SendDialogue();

			int32 temp_line_index = CurrentDialogueLineIndex -1;

			int32 soundCurrentTime = temp_line_index < 0 ? 0 : CurrentDialogue->TimeStamps[temp_line_index].TimeStamp;

			if (bShouldAdjustAudioTiming && AudioComponent->Sound != nullptr && DialogueLineElapsedTime < soundCurrentTime)
			{
				AudioComponent->Play(soundCurrentTime);
				bShouldAdjustAudioTiming = false;
			}

			DialogueLineElapsedTime = soundCurrentTime;
			CurrentDialogueLineIndex++;

			//}

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
		UGameInstance* GameInstance = InWorld.GetGameInstance();

		if (!GameInstance)
		{
			return;
		}

		CreateSubtitlesWidget(InWorld);

		//IF AUDIO ENABLED?
		CreateAudioComponent(InWorld);

#if WITH_EDITOR
		if (SubtitlesWidget->SpeakersDataTable == nullptr)
		{
			FMessageLog("PIE").Error(LOCTEXT("NullSpeakersDataTable", "Please set a data table for speakers in the widget's properties!"));
			bNoSpeakerDataTable = true;
		}
		else
#endif
		SubtitlesWidget->SpeakersDataTable.LoadSynchronous();
	}
}

void USNS_DialogueWorldSubsystem::EnqueueDialogue(const FSNS_Dialogue&& InDialogue)
{
	if (bNoSpeakerDataTable || bIsDisabled || DialoguesToPlay.Contains(InDialogue))
	{
		return;
	}

	DialoguesToPlay.Add(InDialogue);

	if (!bIsPlayingAudio)
	{
		bool AllLinesEnded;
		PlayDialogue(AllLinesEnded);
	}
}

void USNS_DialogueWorldSubsystem::CreateSubtitlesWidget(const UWorld& InWorld)
{
	if (SubtitlesWidgetClass)
	{
		SubtitlesWidget = Cast<USNS_Widget>(CreateWidget(InWorld.GetFirstPlayerController(), SubtitlesWidgetClass));
		SubtitlesWidget->AddToViewport(612);
		SubtitlesWidget->SpeakersDataTable.LoadSynchronous();
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

	FSNS_S_Dialogue* TempDialogue = DialoguesToPlay[0].DialoguesDataTable->FindRow<FSNS_S_Dialogue>(DialoguesToPlay[0].DialogueRowName, "", false);

	if (!TempDialogue)
	{
		FMessageLog("PIE").Error(FText::Format(LOCTEXT("NotFoundRow", "Dialogue row name '{0}' cannot be found!"), FText::FromName(DialoguesToPlay[0].DialogueRowName)));
		return;
	}

	CurrentDialogue = TempDialogue;
	CurrentDialogue->AudioClip.LoadSynchronous();

	if (CurrentDialogue->AudioClip)
	{
		AudioComponent->SetSound(CurrentDialogue->AudioClip.Get());
		AudioComponent->Play(0.f);
	}

	CurrentDialogueLineIndex = 0;

	bIsTickEnabled = true;
	bIsPlayingAudio = true;
}

void USNS_DialogueWorldSubsystem::ManageDialogueEnd()
{
	DialoguesToPlay.RemoveAt(0);

	//stops the audio if the last timestamp dosen't match with it's end
	if (AudioComponent->Sound != nullptr)
	{
		AudioComponent->Stop();
		AudioComponent->Sound = nullptr;
	}

	bIsPlayingAudio = false;

	if (SubtitlesWidget)
	{
		//call ON END CURRENT DIALOGUE LINES (animation to remove subtitle)
		SubtitlesWidget->OnCurrentDialogueEnd();
	}

	//if there aren't other dialogues in "queue" to play
	if (DialoguesToPlay.IsEmpty())
	{
		bIsTickEnabled = false;
		//CALL ON END ALL DIALOGUES
		if (SubtitlesWidget)
		{
			SubtitlesWidget->OnAllDialoguesEnd();
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
	const FName& SpeakerRowName = CurrentDialogue->TimeStamps[CurrentDialogueLineIndex].Speaker.RowName;

	FSNS_S_Speaker* Speaker = SubtitlesWidget->SpeakersDataTable->FindRow<FSNS_S_Speaker>(SpeakerRowName, "", true);

#if WITH_EDITOR
	if (!Speaker)
	{
		FMessageLog("PIE").Error(FText::Format(LOCTEXT("SpeakerNotFound", "Speaker '{0}' cannot be found!"), FText::FromName(SpeakerRowName)));
		UE_LOG(LogTemp, Error, TEXT("SPEAKER %s NOT FOUND!!!!"), *SpeakerRowName.ToString());
		return;
	}
#endif

	DialogueLineRemaningTime += CurrentDialogue->TimeStamps[CurrentDialogueLineIndex].TimeStamp - DialogueLineElapsedTime;

	if (SubtitlesWidget)
	{
		SubtitlesWidget->OnReceivedDialogue(*Speaker, CurrentDialogue->TimeStamps[CurrentDialogueLineIndex].SubtitleText);
	}
}

void USNS_DialogueWorldSubsystem::SkipCurrentLine()
{
	DialogueLineRemaningTime = 0;
	bShouldAdjustAudioTiming = true;
	

	//SendDialogue();
	//CurrentDialogueLineIndex++;

	//AudioComponent->SetPaused(false);
}