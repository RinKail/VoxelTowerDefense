// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BlockDefense/Map/Map_ConstructionGrid.h"
#include "Engine/DataTable.h"
#include "PlayerHudBase.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKDEFENSE_API UPlayerHudBase : public UUserWidget
{
	GENERATED_BODY()
protected:

		UPROPERTY(EditAnywhere, Category = "Table")
		class UDataTable* Table;




	//Data table accessors.
	UFUNCTION(BlueprintCallable, Category = "Buildable Data")
		FBuildableTableRow GetStructureByName(FName RowName);
	UFUNCTION(BlueprintCallable, Category = "Buildable Data")
		FBuildableTableRow GetStructureByIndex(int Index);



	UPROPERTY(BlueprintReadOnly)
		TArray<FName> AvailableItems;

	
};

UCLASS()
class BLOCKDEFENSE_API UInspectWindow : public UPlayerHudBase
{
	GENERATED_BODY()



public:
	UInspectWindow();

	void NewItemSelected(FName ID, class ABuildableBase* Actor);
	void ItemUnselected();

	void ItemHoverStart(FName ID);
	void ItemHoverEnd();
protected: 

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SellBuildable();


	UPROPERTY()
	FBuildableTableRow CurrentItem;
	UPROPERTY()
	bool bItemSelected;
	UPROPERTY()
	bool bItemHovered;
	UPROPERTY(BlueprintReadOnly, Category = "Selection")
	class ABuildableBase* Buildable;




public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	FBuildableTableRow GetSelectedItem() { return CurrentItem; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	bool IsItemSelected() { return bItemSelected; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	bool IsItemHovered() { return bItemHovered; }

		//When an item has been clicked on.
		UFUNCTION(BlueprintImplementableEvent)
		void OnItemSelected(FBuildableTableRow SelectedItem);
	//When the user presses right mouse button while an item is selected.
	UFUNCTION(BlueprintImplementableEvent)
		void OnItemUnselected();
	//When the player mouses over an item.
	UFUNCTION(BlueprintImplementableEvent)
		void OnHighlightStart(FBuildableTableRow HoveredItem);

	//When the player stops mousing over an item.
	UFUNCTION(BlueprintImplementableEvent)
		void OnHighlightEnd();




};


UCLASS()
class BLOCKDEFENSE_API URecipeBar : public UPlayerHudBase
{
	GENERATED_BODY()

	

public: 

	void StructureListUpdated(TArray<FName> In);
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnStructureListRebuild();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCreateItem(FBuildableTableRow Data, FName ID);
	



};

UCLASS()
class BLOCKDEFENSE_API UCompositionDisplay : public UPlayerHudBase
{
	GENERATED_BODY()

	

protected:

	UFUNCTION(BlueprintCallable, Category = "Data")
		void GenerateBlock(FName ID);

	UFUNCTION(BlueprintImplementableEvent)
		void OnCreation(bool bTurret);

	UFUNCTION(BlueprintImplementableEvent)
	void OnBlockUpdated(FVector2D Pos, EBlockType Type);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Data")
	bool IsTurret() { return bIsTurret; }


	UPROPERTY()
	FBlockComposition Composition;
	UPROPERTY()
	bool bIsTurret;



	


};

UCLASS()
class BLOCKDEFENSE_API UResourceBar : public UPlayerHudBase
{
	GENERATED_BODY()

public:

	void UpdateCurrency(int Currency);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Selection")
	EBlockType GetSelectedType() { return SelectedType; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Points")
	int GetCurrentPoints() { return Currency; }
	

	UFUNCTION(BlueprintCallable, Category = "Selection")
		void SetSelectedType(EBlockType Mode) { SelectedType = Mode; }

	void PointsIncreased(int Increase);
	void PointsDecreased(int Decrease);
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Selection")
	EBlockType SelectedType;



	int Currency;



	

protected: 

	//BLUEPRINT EVENTS

	UFUNCTION(BlueprintImplementableEvent)
		void OnPointsIncreased(int IncreaseAmount);
	UFUNCTION(BlueprintImplementableEvent)
		void OnPointsDecreased(int DecreaseAmount);

};
