// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "GameFramework/GameModeBase.h"
#include "BlockDefenseGameMode.generated.h"


USTRUCT(BlueprintType)
struct FTileCluster
{
	GENERATED_BODY()
public:
	FTileCluster()
	{
		ClusterID = 0;
	}
	FTileCluster(int ID, class AMap_Tile_Game* Tile)
	{
		ClusterID = ID;
		Tiles.Add(Tile);
	}

	

	int ClusterID;
	TArray<class AMap_Tile_Game*> Tiles;
};


USTRUCT(BlueprintType)
struct FRoundData
{
	GENERATED_BODY()
public:
	



	UPROPERTY();
	TMap<int, int> IDList;
};


UCLASS(minimalapi)
class ABlockDefenseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	
	

	ABlockDefenseGameMode();


	virtual void BeginPlay() override;


	float GetKineticModifier();
	float GetEnergyModifier();
	float GetPiercingModifier();


	float GetDamageModifier();
	float GetRangeModifier();


	//Gamemode Events
	void StartWave();
	
	void WaveComplete();




	
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnitDestruction();
	UFUNCTION(BlueprintImplementableEvent)
	void OnWaveStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnWaveEnd();
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameOver();

	UFUNCTION(BlueprintCallable, Category = "Visual Effects")
	void TriggerTileUpdateVisualEvent(const FVector& Pos, float Intensity);

	void UpdateLockedTiles();
	void UnlockAdjacentTiles(class AMap_Tile_Game* Tile);



	//Item Activation Events


	void ActivateUpgrade_OnUnitDestroyed();
	void ActivateUpgrade_OnTurretFired();
	void ActivateUpgrade_OnStructureCreated();
	void ActivateUpgrade_OnScoreIncrease();
	void ActivateUpgrade_OnScoreDecrease();


	//Called by Tiles upon Instantiation. Adds them to the TileList and updates Cluster groups.
	void AddTile(class AMap_Tile_Game* Tile);

	//Called by Tiles that are transforming, replaces their reference with the new tile.
	void ReplaceTile(class AMap_Tile_Game* Tile);


	class AEnemyBase* GetUnit(const int ID);

	TArray<class AEnemyBase*>& GetEnemyList() { return UnitList; }

	void GenerateUnitList();

	void UnitDestroyed(class AEnemyBase* Unit);
	

	

	

protected: 


	void GenerateUnit();
	void GetAllEntrances();

	UPROPERTY(EditAnywhere, Category = "Gameplay Setup")
	TArray<TSubclassOf<class AEnemyBase>> EnemyClasses;



	int StealthUnits;
	int ShieldUnits;
	int ArmourUnits;

	UPROPERTY(EditAnywhere, Category = "Gameplay Setup")
	float SpawnInterval;

	//List of Entrance positions, This is where AI are spawned from.
	TArray<FVector> EntrancePositions;

	TArray<class AEnemyBase*> UnitList;
	TArray<int> IDToSpawn;

	bool bWaveComplete;

	UPROPERTY()
	int CurrentWave;
	UPROPERTY()
	int ActiveUnits;

	int TotalUnits;

	UPROPERTY()
	int SpawnIndex;

	int TileScale;

	UPROPERTY(EditDefaultsOnly, Category = "Round Time")
	float RoundInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Round Time")
	float RoundLength;

	UPROPERTY(EditDefaultsOnly, Category = "Round Time")
	float RoundIncrement;

	UPROPERTY()
	TArray<class UUpgradeBase*> AquiredUpgrades;

	TArray<FTileCluster> ClusterGroups;
	TMap <FVector2D, class AMap_Tile_Game*> TileList;


public:

	int GetCurrentActiveUnits() { return ActiveUnits; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Wave")
	int GetCurrentWave() { return CurrentWave; }

private: 


	FTimerHandle TimerHandle_UnitSpawnInterval;

};



