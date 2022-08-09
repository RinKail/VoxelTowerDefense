// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockDefense/Map/Map_ConstructionGrid.h"
#include "BuildableBase.generated.h"

UCLASS()
class BLOCKDEFENSE_API ABuildableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	

	//Virtual functions

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void RemoveFromGrid();

	//Called by the grid onc creation, this provides the structure with the construction grid and performs any setup, Less important for Turrets but intergral for Buildings.
	virtual void Constructed(class AMap_ConstructionGrid* InGrid);

	virtual void Upgrade(EBlockType Type);

// BLUEPRINT EVENTS

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectEnd();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnBuild();

	//Triggered when any grid tile aside from itself is changed.
	//Should be used for any visual events.
	UFUNCTION(BlueprintImplementableEvent)
	void OnTileChanged(FVector TilePosition);




	FName GetItemID() { return ItemID; }

	void SetID(const FName& NewID) { ItemID = NewID; }

	bool IsTurret() { return bIsTurret; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid")
	class AMap_ConstructionGrid* GetGrid() { return Grid; }

	

protected:


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selection")
		FName ItemID;
	UPROPERTY();
	bool bIsTurret;
	class AMap_ConstructionGrid* Grid;
	


};
