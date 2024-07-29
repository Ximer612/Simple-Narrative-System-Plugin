// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_CustomProjectSettings.h"

USNS_CustomProjectSettings::USNS_CustomProjectSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), SpeakersDataTable(nullptr)
, StringSetting("Ximer - Marco Baldini - 2024"), DialogueWidgetBlueprint(nullptr), bSubtitlesEnabled(true)
{
	if (SpeakersDataTable)
	{
		SpeakersDataTable.LoadSynchronous();
	}
}
