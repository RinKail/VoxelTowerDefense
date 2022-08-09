// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/UI/PlayerHudBase.h"
#include "BlockDefense/Map/Map_ConstructionGrid.h"
#include "BlockDefense/Buildables/BuildableBase.h"
#include "BlockDefense/BlockDefensePlayerController.h"
#include "Kismet/GameplayStatics.h"


FBuildableTableRow UPlayerHudBase::GetStructureByName(const FName& RowName)
{
	static const FString ContextString(TEXT("Structure Data"));

	return *Table->FindRow<FBuildableTableRow>(RowName, ContextString, true);
}
FBuildableTableRow UPlayerHudBase::GetStructureByIndex(const int Index)
{
	static const FString ContextString(TEXT("Structure Data"));
	TArray<FName> NameList = Table->GetRowNames();
	if (Index < NameList.Num())
	{
		
		return *Table->FindRow<FBuildableTableRow>(NameList[Index], ContextString, true);
	}
	else
	{
		return FBuildableTableRow();
	}



}
UInspectWindow::UInspectWindow()
{
	Buildable = nullptr;
	bItemSelected = false;
	bItemHovered = false;
}

void UInspectWindow::NewItemSelected(const FName& ID, class ABuildableBase* Actor)
{
	if (bItemSelected)
	{

		ItemUnselected();
		Buildable = nullptr;

	}
	Buildable = Actor;
	CurrentItem = GetStructureByName(ID);
	bItemSelected = true;
	OnItemSelected(CurrentItem);
}
void UInspectWindow::ItemUnselected()
{
	bItemSelected = false;
	OnItemUnselected();
}

void UInspectWindow::ItemHoverStart(const FName& ID)
{
	bItemHovered = true;
	OnHighlightStart(GetStructureByName(ID));
}
void UInspectWindow::ItemHoverEnd()
{
	bItemHovered = false;
	OnHighlightEnd();
}

void UInspectWindow::SellBuildable()
{
	if (IsValid(Buildable))
	{
		//Remove this object from the grid, then call our Unselect event.
		//Removing item from the grid will also destroy the actor.
		Cast<ABlockDefensePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->IncreasePoints(CurrentItem.RefundValue);
		Buildable->RemoveFromGrid();
		ItemUnselected();
		Buildable = nullptr;
	}

	
}


void URecipeBar::StructureListUpdated(TArray<FName>& In)
{
	AvailableItems = In;
	OnStructureListRebuild();

	for (int i = 0; i < AvailableItems.Num(); i++)
	{
		OnCreateItem(GetStructureByName(AvailableItems[i]), AvailableItems[i]);
	}

}

void UCompositionDisplay::GenerateBlock(const FName& ID)
{
	//Get the data from the data table.
	Composition = GetStructureByName(ID).BlockComposition;
	bIsTurret = GetStructureByName(ID).bTurret;
	OnCreation(bIsTurret);
	
	//Depending on whether or not the object it a Turret or a building, the visual grid will work differently.
	//Root position of Buildings is the central cell
	//Root position of turrets is the bottom middle cell.
	if (bIsTurret)
	{
		//Update all cells to be None.
		for (int x = -1; x < 2; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				OnBlockUpdated(FVector2D(x, y), EBlockType::BT_None);



			}
		}
		//Apply the tile data to the visual grid, this is only going to be in the central column.
		

		for (int x = -1; x < 2; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (Composition.Composition.Contains(FVector(0, x, y)))
				{
					OnBlockUpdated(FVector2D(x, y), Composition.Composition[FVector(0, x, y)].Type);
				}
				else
				{
					OnBlockUpdated(FVector2D(x, y), EBlockType::BT_None);
				}



			}
		}
	}
	else
	{

		for (int x = -1; x < 2; x++)
		{
			for (int y = -1; y < 2; y++)
			{
				if (Composition.Composition.Contains(FVector(x, y, 0)))
				{
					OnBlockUpdated(FVector2D(y, x), Composition.Composition[FVector(x, y, 0)].Type);
				}
				else
				{
					OnBlockUpdated(FVector2D(y, x), EBlockType::BT_None);
				}



			}
		}

		
	}
}


void UResourceBar::PointsIncreased(const int Increase)
{
	Currency += Increase;
	OnPointsIncreased(Increase);
}
void UResourceBar::PointsDecreased(const int Decrease)
{
	Currency -= Decrease;
	OnPointsDecreased(Decrease);
}
