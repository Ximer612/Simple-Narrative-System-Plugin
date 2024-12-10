// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#include "SNS_DataTableCustomization.h"

#include "DataTableEditorUtils.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyEditorModule.h"

#define LOCTEXT_NAMESPACE "FSNS_DataTableCustomization"

void FSNS_DataTableCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	DataTablePropertyHandle = InStructPropertyHandle->GetChildHandle("DataTable");
	RowNamePropertyHandle = InStructPropertyHandle->GetChildHandle("RowName");

	FPropertyComboBoxArgs ComboArgs(RowNamePropertyHandle,
		FOnGetPropertyComboBoxStrings::CreateSP(this, &FSNS_DataTableCustomization::OnGetRowStrings),
		FOnGetPropertyComboBoxValue::CreateSP(this, &FSNS_DataTableCustomization::OnGetRowValueString));

	ComboArgs.ShowSearchForItemCount = 1;

	TSharedRef<SWidget> BrowseTableButton = PropertyCustomizationHelpers::MakeBrowseButton(
		FSimpleDelegate::CreateSP(this, &FSNS_DataTableCustomization::BrowseTableButtonClicked),
		LOCTEXT("SsBrowseToDatatable", "Browse to DataTable in Content Browser"));
	HeaderRow
		.NameContent()
		[
			InStructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		.MaxDesiredWidth(0.0f) // don't constrain the combo button width
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				[
					PropertyCustomizationHelpers::MakePropertyComboBox(ComboArgs)
				]
				+ SHorizontalBox::Slot()
				.Padding(2.0f)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					BrowseTableButton
				]
		]; ;

	FDataTableEditorUtils::AddSearchForReferencesContextMenu(HeaderRow, FExecuteAction::CreateSP(this, &FSNS_DataTableCustomization::OnSearchForReferences));
}

void FSNS_DataTableCustomization::BrowseTableButtonClicked()
{
	if (DataTablePropertyHandle.IsValid())
	{
		UObject* SourceDataTable = nullptr;
		if (DataTablePropertyHandle->GetValue(SourceDataTable) == FPropertyAccess::Success)
		{
			TArray<FAssetData> Assets;
			Assets.Add(SourceDataTable);
			GEditor->SyncBrowserToObjects(Assets);
		}
	}
}

bool FSNS_DataTableCustomization::GetCurrentValue(UDataTable*& OutDataTable, FName& OutName) const
{
	if (RowNamePropertyHandle.IsValid() && RowNamePropertyHandle->IsValidHandle() && DataTablePropertyHandle.IsValid() && DataTablePropertyHandle->IsValidHandle())
	{
		// If either handle is multiple value or failure, fail
		UObject* SourceDataTable = nullptr;
		if (DataTablePropertyHandle->GetValue(SourceDataTable) == FPropertyAccess::Success)
		{
			OutDataTable = Cast<UDataTable>(SourceDataTable);

			if (RowNamePropertyHandle->GetValue(OutName) == FPropertyAccess::Success)
			{
				return true;
			}
		}
	}
	return false;
}

void FSNS_DataTableCustomization::OnSearchForReferences()
{
	UDataTable* DataTable;
	FName RowName;

	if (GetCurrentValue(DataTable, RowName) && DataTable)
	{
		TArray<FAssetIdentifier> AssetIdentifiers;
		AssetIdentifiers.Add(FAssetIdentifier(DataTable, RowName));

		FEditorDelegates::OnOpenReferenceViewer.Broadcast(AssetIdentifiers, FReferenceViewerParams());
	}
}

FString FSNS_DataTableCustomization::OnGetRowValueString() const
{
	if (!RowNamePropertyHandle.IsValid() || !RowNamePropertyHandle->IsValidHandle())
	{
		return FString();
	}

	FName RowNameValue;
	const FPropertyAccess::Result RowResult = RowNamePropertyHandle->GetValue(RowNameValue);
	if (RowResult == FPropertyAccess::Success)
	{
		if (RowNameValue.IsNone())
		{
			return LOCTEXT("DataTable_None", "None").ToString();
		}
		return RowNameValue.ToString();
	}
	else if (RowResult == FPropertyAccess::Fail)
	{
		return LOCTEXT("DataTable_None", "None").ToString();
	}
	else
	{
		return LOCTEXT("MultipleValues", "Multiple Values").ToString();
	}
}

void FSNS_DataTableCustomization::OnGetRowStrings(TArray< TSharedPtr<FString> >& OutStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems) const
{
	UDataTable* DataTable = nullptr;
	FName IgnoredRowName;

	// Ignore return value as we will show rows if table is the same but row names are multiple values
	GetCurrentValue(DataTable, IgnoredRowName);

	TArray<FName> AllRowNames;
	if (DataTable != nullptr)
	{
		for (TMap<FName, uint8*>::TConstIterator Iterator(DataTable->GetRowMap()); Iterator; ++Iterator)
		{
			AllRowNames.Add(Iterator.Key());
		}

		// Sort the names alphabetically.
		AllRowNames.Sort(FNameLexicalLess());
	}

	for (const FName& RowName : AllRowNames)
	{
		OutStrings.Add(MakeShared<FString>(RowName.ToString()));
		OutRestrictedItems.Add(false);
	}
}


#undef LOCTEXT_NAMESPACE