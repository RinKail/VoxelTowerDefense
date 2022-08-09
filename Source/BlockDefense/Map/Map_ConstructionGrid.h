// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Map_ConstructionGrid.generated.h"


UENUM(BlueprintType)
enum class EBlockType : uint8
{
	BT_None      UMETA(DisplayName = "None"),


	BT_Red			UMETA(DisplayName = "Red"),
	BT_Green	 UMETA(DisplayName = "Green"),
	BT_Blue		 UMETA(DisplayName = "Blue"),
};

USTRUCT(BlueprintType)
struct FGridData
{
	GENERATED_BODY()
public:

	UPROPERTY()
		FVector Position;
	UPROPERTY(EditAnywhere, Category = "Data")
		EBlockType Type;

		class AConstructionBlock* Actor;


};

USTRUCT(BlueprintType)
struct FConstructData
{
	GENERATED_BODY()
public:

	UPROPERTY()
		FVector Position;


	class ABuildableBase* Actor;


};

USTRUCT(BlueprintType)
struct FBlockComposition
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Data")
		TMap<FVector, FGridData> Composition;
	UPROPERTY(EditAnywhere, Category = "Data")
	TSubclassOf<class ABuildableBase> Class;

};








//Provides a turret and a block composition, Blocks will check against this to determine if a buildable should be made, and what that buildable is.
USTRUCT(BlueprintType)
struct FBuildableTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:

	FBuildableTableRow()
	{
		Name = FName(TEXT("None"));
		Description = "None";
		RefundValue = 0;
	}


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Composition")
		FBlockComposition BlockComposition;

	

	//Name, used by UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FName Name;
	//Description, used by UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int RefundValue;

	//Determines if this item is a turret. if false, then this structure is a building instead.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		bool bTurret;

};




UCLASS()
class BLOCKDEFENSE_API AMap_ConstructionGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMap_ConstructionGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int TileScale;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<class AConstructionBlock> BlockClass;
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	UDataTable* Table;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<class URecipeBar> RecipeBarClass;
	UPROPERTY()
	class URecipeBar* WidgetRecipeBar;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Adds a block to the grid space, spawns it into the world space and applies the data. 
	//Will Stack blocks in the case the position is already occupied.
	ABuildableBase* AddBlock(const FVector& Position, EBlockType Type, bool bOnRootTile);
	//Removes block at the exact location provided.
	void RemoveBlock(const FVector& Position);

	void RemoveBuildable(const FVector& Position);
	

	//Should be called at the end of Construction block visual effects and after it's been placed completely;
	UFUNCTION(BlueprintCallable, Category = "Initialisation")
	void CompletePlacement(AConstructionBlock* KeyTester);

	

	//Searches through the construction grid and attempts to find BlockCombinations that match the available recipes. 
	//On Success, will create the corresponding actor and add it to the grid space while removing the blocks from that cluster.
	void FindAndCreateValidStructure(TArray<FBlockComposition>& StructureList);


	//Returns any buildable at the location provided.
	class ABuildableBase* GetBuidableAt(const FVector& Pos);

	int GetTileScale() { return TileScale; }

	
	//Contains a list of block compositions that equate to a Turret.
	UPROPERTY(EditAnywhere, Category = "Construction Data")
	TArray<FBlockComposition> TurretDataList;
	//Contains a list of block compositions that equate to a Turret.
	UPROPERTY(EditAnywhere, Category = "Construction Data")
	TArray<FBlockComposition> BuildingDataList;

	//Returns all structures in range. This range is based on grid values, an example of a valid range would be [3|5]. Range begins from the root position.
	//ToDo- Make it so that range is calculated from every position the build takes up.
	TArray<class ABuildableBase*> GetStructuresInRange(const int Range, class ABuildableBase* Source, bool bTurret);
	
	//Adds a new structure to the list of Structure recipes.
	UFUNCTION(BlueprintCallable, Category = "Available Structures")
	void AddAvailableStructure(const FName& RowName);
	//Removes a structure from the list of structure recipes.
	UFUNCTION(BlueprintCallable, Category = "Available Structures")
	void RemoveAvailableStructure(const FName& RowName);
	//Rebuilds the structure list, takes available Recipe names and collects their composition data from the provided data table.
	UFUNCTION(BlueprintCallable, Category = "Available Structures")
	void RebuildStructureLists();

	UFUNCTION(BlueprintCallable, Category = "Structure Data")
	int GetTotalStructures() { return StructureGrid.Num(); }

	

private: 

	//Blueprint based Building construction testing can only be completed by the current Key Owner.
	UPROPERTY()
	class AConstructionBlock* ConstructionKey;

	//Grid data that contains all construction blocks. This is used for generating new towers based on Block combinations.
	TMap<FVector, FGridData> ConstructionGrid;

	//Positions of all turrets in grid space.
	TMap<FVector2D, ABuildableBase*> StructureGrid;
	//Locations of all buildable blocks considered roots.
	//For a block to be considered a root, it must be placed directly ontop of a Buildable Platform, not a standard platform.
	TArray<FGridData> RootList;


	//Generates a structure of Construction blocks with their types and relative grid locations to the root. Only collects blocks directly adjacent in Cardinal Directions.
	void GenerateComposition(const FVector& Position, FBlockComposition& Data);

	UPROPERTY()
	TArray<FName> AvailableStructures;


};
