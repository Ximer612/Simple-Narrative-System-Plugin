#include "Structs/SNS_S_Dialogue.h"

void FSNS_S_Dialogue::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	for (int32 i = 0; i < TimeStamps.Num(); i++)
	{
		TimeStamps[i].Speaker.DataTable = SpeakersDataTable;
	}
}
