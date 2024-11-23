// Copyright 2024, Ximer - Marco Baldini, All rights reserved

#pragma once

/**
 * 
 */
class SIMPLENARRATIVESYSTEMEDITOR_API FSNS_DataTableCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FSNS_DataTableCustomization);
	}

	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	// Not needed, but must be implemented
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}

protected:
	bool GetCurrentValue(UDataTable*& OutDataTable, FName& OutName) const;
	void OnSearchForReferences();
	void OnGetRowStrings(TArray< TSharedPtr<FString> >& OutStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems) const;
	FString OnGetRowValueString() const;
	void BrowseTableButtonClicked();

	TSharedPtr<IPropertyHandle> DataTablePropertyHandle;
	TSharedPtr<IPropertyHandle> RowNamePropertyHandle;
	
};
