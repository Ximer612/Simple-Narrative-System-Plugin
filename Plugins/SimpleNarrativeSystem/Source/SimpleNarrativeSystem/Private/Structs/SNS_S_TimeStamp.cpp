#include "Structs/SNS_S_TimeStamp.h"


FSNS_S_TimeStamp::FSNS_S_TimeStamp()
{
	TSoftObjectPtr<UDataTable> SpeakersDataTable = GetDefault<USNS_CustomProjectSettings>()->SpeakersDataTable;
	if (!SpeakersDataTable.IsValid())
	{
		SpeakersDataTable.LoadSynchronous();
	}

	Speaker.DataTable = SpeakersDataTable.Get();

}

FSNS_S_TimeStamp::~FSNS_S_TimeStamp()
{
}
