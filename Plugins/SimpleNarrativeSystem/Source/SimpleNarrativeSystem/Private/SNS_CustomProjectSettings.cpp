// Copyright 2024, Ximer - Marco Baldini, All rights reserved


#include "SNS_CustomProjectSettings.h"

USNS_CustomProjectSettings::USNS_CustomProjectSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
, SpeakersDataTable(nullptr)
, DialogueWidgetBlueprint(nullptr)
,bSubtitlesEnabled(true)
, ZOrder(612)
, StringSetting("Ximer - Marco Baldini - 2024")
{
	if (SpeakersDataTable)
	{
		SpeakersDataTable.LoadSynchronous();
	}
}
