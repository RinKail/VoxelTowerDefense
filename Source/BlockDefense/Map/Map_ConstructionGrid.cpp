// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/Map/Map_ConstructionGrid.h"
#include "BlockDefense/Buildables/Buildable_Turret.h"
#include "BlockDefense/Buildables/ConstructionBlock.h"
#include "BlockDefense/UI/PlayerHudBase.h"
#include "Engine/World.h"

// Sets default values
AMap_ConstructionGrid::AMap_ConstructionGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TileScale = 200;
	ConstructionKey = nullptr;
}

// Called when the game starts or when spawned
void AMap_ConstructionGrid::BeginPlay()
{
	Super::BeginPlay();
	//Create the Recipe bar widget and add it to the screen while storing a reference to it.
	WidgetRecipeBar = CreateWidget<URecipeBar>(GetWorld(), RecipeBarClass, FName(TEXT("RecipeBar")));
	WidgetRecipeBar->AddToViewport();

	
	
}

// Called every frame
void AMap_ConstructionGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMap_ConstructionGrid::CompletePlacement(AConstructionBlock* KeyTester)
{
	//Test if the incoming Actor is the current KeyHolder.
	if (KeyTester == ConstructionKey)
	{
		//This Actor is the current KeyHolder, final block placements must be complete so testing can commence.
		FindAndCreateValidStructure(TurretDataList);
		FindAndCreateValidStructure(BuildingDataList);
	}

	
}
ABuildableBase* AMap_ConstructionGrid::AddBlock(const FVector& Position, EBlockType Type, bool bOnRootTile)
{
	
	
	if (StructureGrid.Contains(FVector2D(Position.X, Position.Y) / TileScale))		
	{ 
		//Is the position in question currently occupied by a structure? Upgrade the structure (CUT DUE TO TIME CONSTRAINTS)
		StructureGrid[FVector2D(Position.X, Position.Y) / TileScale]->Upgrade(Type);
	}
	else if (ConstructionGrid.Contains(Position / TileScale))					
	{
		//Is the position in question currently occupied by a ConstructionBlock? Move one space upwards, no longer considered a root tile.
		return AddBlock(Position + FVector(0,0, TileScale), Type, false);
	}
	else          
	{
		
		//A valid Location has been found, creating a ConstructionBlock.
		//Populate the construction data
		FGridData Temp;
		Temp.Type = Type;
		Temp.Position = FVector((int)Position.X, (int)Position.Y, (int)Position.Z) / TileScale;
		UE_LOG(LogTemp, Warning, TEXT("Adding Block: [%f | %f | %f]"), Temp.Position.X, Temp.Position.Y, Temp.Position.Z);
		ConstructionKey = GetWorld()->SpawnActor<AConstructionBlock>(BlockClass, Position, FRotator(0, 0, 0));
		Temp.Actor = ConstructionKey;		//Used to make sure only the final block being generated can trigger the recipe testing.
		Temp.Actor->Constructed(this);
		Temp.Actor->SetType(Type);
		ConstructionGrid.Add(Temp.Position, Temp);
		
		if (bOnRootTile)
		{
			RootList.Add(Temp);	
		}
		
		

		return Temp.Actor;
	}
	return nullptr;
}

void AMap_ConstructionGrid::RemoveBlock(const FVector& Position)
{
	UE_LOG(LogTemp, Warning, TEXT("Removing Block: [%f | %f | %f]"), Position.X, Position.Y, Position.Z);

	//Does the position provided contain a Block?
	if (ConstructionGrid.Contains(Position))
	{
		//Destroy the actor then remove the containing map index.
		ConstructionGrid[Position].Actor->Destroy();
		ConstructionGrid.Remove(Position);
		//Was the position a root? If so, remove it.
		
		for (int i = 0; i < RootList.Num(); i++)
		{
			if (RootList[i].Position == Position)
			{
				RootList.RemoveAt(i);
				break;
			}
		}
	}
}

void AMap_ConstructionGrid::RemoveBuildable(const FVector& Position)
{
	//Does the structure grid contain this item?
	if (StructureGrid.Contains(FVector2D(Position / TileScale)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found a removeable object, removing."));
		//An entry has been found inside the map, is the item actually valid?
		if (StructureGrid[FVector2D(Position / TileScale)] != nullptr)
		{
			//The item is valid, remove it.
			UE_LOG(LogTemp, Warning, TEXT("Object Name: %s"),*StructureGrid[FVector2D(Position / TileScale)]->GetName());
			StructureGrid[FVector2D(Position / TileScale)]->Destroy();
		}
		//Finally, remove it from the map.
		StructureGrid.Remove(FVector2D(Position / TileScale));

	}
}

void AMap_ConstructionGrid::FindAndCreateValidStructure(TArray<FBlockComposition>& StructureList)
{
	for (int i = 0; i < RootList.Num(); i++)
	{
		//Create a Test composition and pass it into the generator, this will propogate across all existing adjacent tiles until none remain and return the resulting structure.
		FBlockComposition TestComp;
		GenerateComposition(RootList[i].Position, TestComp);
		FVector RootPosition = RootList[i].Position;
		bool bStructureCreated = false;

		//Itterate across the provided structure list to find matching Block combinations.
		for (int o = 0; o < StructureList.Num(); o++)
		{
			int MatchingPoints = 0;
			for (const TPair<FVector, FGridData >& pair : TestComp.Composition)
			{
				 if (StructureList[o].Composition.Contains(pair.Value.Position - (RootPosition)))
				{
					 if (StructureList[o].Composition[pair.Value.Position - RootPosition].Type == pair.Value.Type)
					 {
						 MatchingPoints++;
					 }
				}
			}
			//For a structure to succeed, a 100% match must be found within the provided composition
			//TODO- Refund any blocks deleted that are adjacent but not within the composition.
			if (MatchingPoints == StructureList[o].Composition.Num())
			{
				ABuildableBase* BuiltStructure = GetWorld()->SpawnActor<ABuildableBase>(StructureList[o].Class, RootPosition * TileScale, FRotator(0, 0, 0));
				BuiltStructure->Constructed(this);
				if (StructureGrid.Contains(FVector2D(RootPosition)))
				{
					StructureGrid[FVector2D(RootPosition)] = BuiltStructure;
				}
				else
				{
					StructureGrid.Add(FVector2D(RootPosition), BuiltStructure);
				}
				for (const TPair<FVector, FGridData >& pair : TestComp.Composition)
				{
					RemoveBlock(pair.Value.Position);
				}
				bStructureCreated = true;
				break;
			}
		}
		if (bStructureCreated) break;
	}
}

void AMap_ConstructionGrid::GenerateComposition(const FVector& Position, FBlockComposition& Data)
{
	//Recursively loop through adjacent tiles, Stops at previously tested or invalid tiles.
	if (!Data.Composition.Contains(Position) && ConstructionGrid.Contains(Position))
	{
		Data.Composition.Add(Position, ConstructionGrid[Position]);
		GenerateComposition(Position + FVector(1, 0, 0), Data);
		GenerateComposition(Position + FVector(-1, 0, 0), Data);
		GenerateComposition(Position + FVector(0, -1, 0), Data);
		GenerateComposition(Position + FVector(0, 1, 0), Data);
		GenerateComposition(Position + FVector(0, 0, 1), Data);
	}
}

//Returns either a ConstructionBlock or Turret/Building at given position
//If returning a construction block, only returns top most block.
//Returns nullptr on fail, functionality must account for this.
ABuildableBase* AMap_ConstructionGrid::GetBuidableAt(const FVector& Pos)
{
	Pos /= TileScale;
	Pos.Z = 1;


	if (ConstructionGrid.Contains(Pos))
	{
		bool bItemExists = true;
		FVector TestPosition = Pos;
		//The lowest position in the grid contains a block, are there any blocks above?
		while (bItemExists)
		{
			if (ConstructionGrid.Contains(TestPosition + FVector(0, 0, 1)))
			{
				TestPosition += FVector(0,0, 1);
			}
			else
			{
				bItemExists = false;
				break;
			}
		}
		return ConstructionGrid[TestPosition].Actor;
	}
	else if (StructureGrid.Contains(FVector2D(Pos.X, Pos.Y)))
	{	//There were no blocks inside the construction grid, but there was an item in the structure grid, return the structure found.
		
		return StructureGrid[FVector2D(Pos.X, Pos.Y)];
	}
	else
	{
		return nullptr;
	}
	return nullptr;
}


//Returns all structures in range, will return either a Turret or a building but not both.
TArray<ABuildableBase*> AMap_ConstructionGrid::GetStructuresInRange(const int Range, ABuildableBase* Source, bool bTurret)
{
	TArray<class ABuildableBase*> StructuresInRange;
	FVector2D GridPos = FVector2D(Source->GetActorLocation() / TileScale);
	for (const TPair<FVector2D, ABuildableBase* >& pair : StructureGrid)
	{
		FVector2D Pos = GridPos - FVector2D(pair.Value->GetActorLocation() / TileScale);
		if (Pos.X > -Range && Pos.X < Range &&
			Pos.Y > -Range && Pos.Y < Range
			)
		{
			if (pair.Value->IsTurret() && bTurret)
			{
				StructuresInRange.Add(pair.Value);
			}
			else if (!pair.Value->IsTurret() && !bTurret)
			{
				StructuresInRange.Add(pair.Value);
			}
		}
	}
	return StructuresInRange;
}

void AMap_ConstructionGrid::AddAvailableStructure(const FName& RowName)
{
	if (!AvailableStructures.Contains(RowName))
	{
		AvailableStructures.Add(RowName);
		RebuildStructureLists();
	}
}
void AMap_ConstructionGrid::RemoveAvailableStructure(const FName& RowName)
{
	if (AvailableStructures.Contains(RowName))
	{
		AvailableStructures.Remove(RowName);
		RebuildStructureLists();
	}
}
void AMap_ConstructionGrid::RebuildStructureLists()
{
	//Accesses the data table and rebuilds the available recipes based on the AvailableItem list.
	//Available item list should be updateable by buildings, and will be the primary method of progression
	static const FString ContextString(TEXT("Structure Data"));
	TurretDataList.Empty();
	BuildingDataList.Empty();
	for (int i = 0; i < AvailableStructures.Num(); i++)
	{
		FBuildableTableRow* TableRow = Table->FindRow<FBuildableTableRow>(AvailableStructures[i], ContextString, true);
		if (TableRow)
		{
			if (TableRow->bTurret)
			{
				TurretDataList.Add(TableRow->BlockComposition);
			}
			else
			{
				BuildingDataList.Add(TableRow->BlockComposition);
			}
		}
	}

	WidgetRecipeBar->StructureListUpdated(AvailableStructures);
}
