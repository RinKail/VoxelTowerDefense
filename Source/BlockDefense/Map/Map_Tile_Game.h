// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockDefense/Map/Map_Tile.h"
#include "Map_Tile_Game.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKDEFENSE_API AMap_Tile_Game : public AMap_Tile
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "Tile Transformation")
		TSubclassOf<AMap_Tile_Game> ClassToBecome;

	UPROPERTY(EditAnywhere, Category = "Tile Transformation")
		int TransformationCost;

	UPROPERTY(EditAnywhere, Category = "Tile Gameplay")
		float TransformTime;

	//Timer handle for tile transformations.
	FTimerHandle TimerHandle_TileTransformation;

	UPROPERTY(BlueprintReadOnly, Category = "Tile Gameplay")
	bool bTransformConditionsMet;

public:

	AMap_Tile_Game();


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TileState")
		bool CanTileTransform(const int Currency);

	int GetCost() { return TransformationCost; }

	virtual void TransformTile(ABlockDefensePlayerController* Controller);
	virtual void TransformTile_Complete();


	UFUNCTION(BlueprintImplementableEvent)
	void OnTransformationTriggered();
	UFUNCTION(BlueprintImplementableEvent)
	void OnTransformationComplete();
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameStart();

	UFUNCTION(BlueprintImplementableEvent)
		void OnCreation();




};
