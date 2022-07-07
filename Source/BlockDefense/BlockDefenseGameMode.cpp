// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlockDefenseGameMode.h"
#include "BlockDefensePlayerController.h"
#include "BlockDefenseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BlockDefense/Map/Map_EnemyEntrance.h"
#include "BlockDefense/AI/EnemyBase.h"
#include "BlockDefense/Map/Map_Tile.h"
#include "BlockDefense/Map/Map_Tile_Game.h"
#include "BlockDefense/Map/Map_Tile_Editor.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

ABlockDefenseGameMode::ABlockDefenseGameMode()
{
	




	CurrentWave = 0;
	bWaveComplete = false;
}


void ABlockDefenseGameMode::BeginPlay()
{
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMap_Tile_Editor::StaticClass(), ActorList);

	for (int i = 0; i < ActorList.Num(); i++)
	{
		AddTile(Cast<AMap_Tile_Editor>(ActorList[i])->GenerateGameplayActor());
		ActorList[i]->Destroy();
	}

	UpdateLockedTiles();
	GetAllEntrances();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_UnitSpawnInterval, this, &ABlockDefenseGameMode::StartWave, RoundInterval);
	GenerateUnitList();
	Super::BeginPlay();
	
}

void ABlockDefenseGameMode::StartWave()
{
	bWaveComplete = false;
	GenerateUnit();
	OnWaveStart();
}

void ABlockDefenseGameMode::WaveComplete()
{
	OnWaveEnd();
	RoundLength++;
	CurrentWave++;
	GenerateUnitList();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_UnitSpawnInterval, this, &ABlockDefenseGameMode::StartWave, RoundInterval);
}




void ABlockDefenseGameMode::AddTile(class AMap_Tile_Game* Tile)
{
	TileScale = Tile->GetTileScale();
	FVector2D Key = FVector2D(Tile->GetActorLocation().X / Tile->GetTileScale(), Tile->GetActorLocation().Y / Tile->GetTileScale());
	int ClusterChannel = Tile->GetClusterChannel();
	
	if (ClusterChannel != 0)
	{
		bool bFailedToFindGroup = true;
		for (int i = 0; i < ClusterGroups.Num(); i++)
		{
			if (ClusterGroups[i].ClusterID == ClusterChannel)
			{
				ClusterGroups[i].Tiles.Add(Tile);
				bFailedToFindGroup = false;
				break;
			}
		}
		if (bFailedToFindGroup)
		{
			
			ClusterGroups.Add(FTileCluster(ClusterChannel, Tile));
		}
	}
	TileList.Add(Key, Tile);
	Tile->OnGameStart();



}


void ABlockDefenseGameMode::ReplaceTile(class AMap_Tile_Game* Tile)
{
	TileScale = Tile->GetTileScale();
	FVector2D Key = FVector2D(Tile->GetActorLocation().X / Tile->GetTileScale(), Tile->GetActorLocation().Y / Tile->GetTileScale());
	int ClusterChannel = Tile->GetClusterChannel();
	UpdateLockedTiles();
	
	TileList[Key] = Tile;
}

void ABlockDefenseGameMode::TriggerTileUpdateVisualEvent(FVector Pos, float Intensity)
{
	FVector2D RootPos = FVector2D(Pos / TileScale);
	for (const TPair<FVector2D, AMap_Tile* >& pair : TileList)
	{

		if (pair.Key != RootPos)
		{
			pair.Value->OnTileChanged(Pos, Intensity);
		}
	}
}
	
	



void ABlockDefenseGameMode::GetAllEntrances()
{
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMap_EnemyEntrance::StaticClass(), ActorList);

	for (int i = 0; i < ActorList.Num(); i++)
	{
		EntrancePositions.Add(ActorList[i]->GetActorLocation());
		
	}
}



void ABlockDefenseGameMode::UpdateLockedTiles()
{
	

	for (const TPair<FVector2D, AMap_Tile* >& pair : TileList)
	{


		FVector2D CurrentPosition = pair.Key;
		AMap_Tile* Temp = TileList[CurrentPosition];
	
		if (Temp->GetTileMode() == ETileMode::TM_Platform || Temp->GetTileMode() == ETileMode::TM_Buildable)
		{
			Temp->SetLockState(false);
			if (TileList.Contains(CurrentPosition + FVector2D(0, 1)) && TileList[CurrentPosition + FVector2D(0, 1)]->GetTileMode() == ETileMode::TM_Resource)
			{
				TileList[CurrentPosition + FVector2D(0, 1)]->SetLockState(false);
			}
			if (TileList.Contains(CurrentPosition + FVector2D(0, -1)) && TileList[CurrentPosition + FVector2D(0, -1)]->GetTileMode() == ETileMode::TM_Resource)
			{
				TileList[CurrentPosition + FVector2D(0, -1)]->SetLockState(false);
			}
			if (TileList.Contains(CurrentPosition + FVector2D(1, 0)) && TileList[CurrentPosition + FVector2D(1, 0)]->GetTileMode() == ETileMode::TM_Resource)
			{
				TileList[CurrentPosition + FVector2D(1, 0)]->SetLockState(false);
			}
			if (TileList.Contains(CurrentPosition + FVector2D(-1, 0)) && TileList[CurrentPosition + FVector2D(-1, 0)]->GetTileMode() == ETileMode::TM_Resource)
			{
				TileList[CurrentPosition + FVector2D(-1, 0)]->SetLockState(false);
			}
		}
	}
}

void ABlockDefenseGameMode::UnlockAdjacentTiles(AMap_Tile_Game* Tile)
{
	
	FVector2D CurrentPosition = FVector2D(Tile->GetActorLocation().X / Tile->GetTileScale(), Tile->GetActorLocation().Y / Tile->GetTileScale());

	if (Tile->GetTileMode() == ETileMode::TM_Platform || Tile->GetTileMode() == ETileMode::TM_Buildable)
	{
		if (TileList.Contains(CurrentPosition + FVector2D(0, 1)) && TileList[CurrentPosition + FVector2D(0, 1)]->GetTileMode() == ETileMode::TM_Resource)
		{
			TileList[CurrentPosition + FVector2D(0, 1)]->SetLockState(false);
		}
		if (TileList.Contains(CurrentPosition + FVector2D(0, -1)) && TileList[CurrentPosition + FVector2D(0, -1)]->GetTileMode() == ETileMode::TM_Resource)
		{
			TileList[CurrentPosition + FVector2D(0, -1)]->SetLockState(false);
		}
		if (TileList.Contains(CurrentPosition + FVector2D(-1, 0)) && TileList[CurrentPosition + FVector2D(-1, 0)]->GetTileMode() == ETileMode::TM_Resource)
		{
			TileList[CurrentPosition + FVector2D(-1, 0)]->SetLockState(false);
		}
		if (TileList.Contains(CurrentPosition + FVector2D(1, 0)) && TileList[CurrentPosition + FVector2D(1, 0)]->GetTileMode() == ETileMode::TM_Resource)
		{
			TileList[CurrentPosition + FVector2D(1, 0)]->SetLockState(false);
		}
	}
}


//////////////////////////////////////////
/// 

void ABlockDefenseGameMode::GenerateUnitList()
{
	int TotalGrunts = (CurrentWave * 2) + 10;
	int TotalScouts = 0;
	int TotalHeavys = 0;
	int TotalCarriers = 0;

	
	if ((CurrentWave - 3) > 0)
	{
		TotalScouts = (CurrentWave / 3) * 3;
		ShieldUnits = 3 + (CurrentWave / 3) * 2;
		if ((CurrentWave % 10) == 0)
		{
			TotalScouts *= 1.25;
			ShieldUnits *= 1.2;

		}

	}
	if ((CurrentWave - 4) > 0)
	{
		TotalHeavys = (CurrentWave / 4) * 4;
	

		ArmourUnits = 2 + (CurrentWave / 2);

		if ((CurrentWave % 10) == 0)
		{
			TotalHeavys *= 1.25;
			ArmourUnits *= 1.2;
		}
	}
	if ((CurrentWave - 10) > 0)
	{
		TotalCarriers = (CurrentWave / 10) * 5;
		StealthUnits = 3 + (CurrentWave / 5);
		if ((CurrentWave % 10) == 0)
		{
			TotalCarriers *= 1.25;
			StealthUnits *= 1.1;
		}
	}

	ActiveUnits = TotalGrunts + TotalScouts + TotalHeavys + TotalCarriers;
	TotalUnits = ActiveUnits;
	int Index = 0;
	IDToSpawn.Empty();
	UnitList.Empty();
	SpawnIndex = 0;
	//Distribute the units throughout the wave.
	while ((TotalGrunts + TotalScouts + TotalHeavys + TotalCarriers) > 0)
	{
		if (Index % 4 == 0)
		{
			if (TotalHeavys > 0)
			{
				IDToSpawn.Add(2);
				TotalHeavys--;
			}
			else
			{
				IDToSpawn.Add(0);
				TotalGrunts--;
			}
		}
		else if (Index % 3 == 0)
		{
			if (TotalScouts > 0)
			{
				IDToSpawn.Add(1);
				TotalScouts--;
			}
			else
			{
				IDToSpawn.Add(0);
				TotalGrunts--;
			}
		}
		else if (Index % 10 == 0)
		{
			if (TotalCarriers > 0)
			{
				IDToSpawn.Add(3);
				TotalCarriers--;
			}
			else
			{
				IDToSpawn.Add(0);
				TotalGrunts--;
			}
		}
		else
		{
			IDToSpawn.Add(0);
			TotalGrunts--;

		}
		Index++;
	}


	
}


void ABlockDefenseGameMode::UnitDestroyed(AEnemyBase* Unit)
{
	
	if ( IsValid(Unit))
	{
		UnitList.Remove(Unit);
		ActiveUnits--;
		UE_LOG(LogTemp, Warning, TEXT("Units Remaining: %d"), ActiveUnits);
		
		if (ActiveUnits <= 0)
		{
			bWaveComplete = true;
			WaveComplete();
		}

		Unit->Destroy();
	}
}



void ABlockDefenseGameMode::GenerateUnit()
{
	if (SpawnIndex < TotalUnits)
	{
		AEnemyBase* GeneratedUnit = GetWorld()->SpawnActor<AEnemyBase>(EnemyClasses[IDToSpawn[SpawnIndex]], EntrancePositions[0], FRotator(0,0,0));
		SpawnIndex++;
		if (GeneratedUnit  != nullptr)
		{
			UnitList.Add(GeneratedUnit);

			int TotalTypes = StealthUnits + ArmourUnits + ShieldUnits;
			int NormalUnits = (TotalUnits - TotalTypes);
			TotalTypes += NormalUnits;
			int SelectedType = FMath::RandRange(0, TotalTypes);
			if (SelectedType <= NormalUnits)
			{
				GeneratedUnit->SetBodyType(EBlockType::BT_None);
			}
			else if (SelectedType < NormalUnits + StealthUnits)
			{
				GeneratedUnit->SetBodyType(EBlockType::BT_Blue);
			}
			else if (SelectedType < NormalUnits + StealthUnits + ArmourUnits)
			{
				GeneratedUnit->SetBodyType(EBlockType::BT_Red);
			}
			else if (SelectedType < NormalUnits + StealthUnits + ArmourUnits + ShieldUnits)
			{
				GeneratedUnit->SetBodyType(EBlockType::BT_Green);
			}
			GeneratedUnit->InitialiseValues(CurrentWave);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_UnitSpawnInterval, this, &ABlockDefenseGameMode::GenerateUnit, RoundLength / TotalUnits);
		}
		
		
		
	}



}
