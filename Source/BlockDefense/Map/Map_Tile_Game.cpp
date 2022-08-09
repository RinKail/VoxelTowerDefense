// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/Map/Map_Tile_Game.h"
#include "BlockDefense/BlockDefensePlayerController.h"
#include "BlockDefense/BlockDefenseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"



AMap_Tile_Game::AMap_Tile_Game()
{
	bLockedState = true;
	bTransformConditionsMet = false;

}

void AMap_Tile_Game::TransformTile(ABlockDefensePlayerController* Controller)
{
	ControllerReference = Controller;
	Controller->TransformTile_Start();
	OnTransformationTriggered();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_TileTransformation, this, &AMap_Tile_Game::TransformTile_Complete, TransformTime);
}
void AMap_Tile_Game::TransformTile_Complete()
{
	ControllerReference->TransformTile_End();
	OnTransformationComplete();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMap_Tile_Game* Temp = GetWorld()->SpawnActor<AMap_Tile_Game>(ClassToBecome, GetActorLocation(), GetActorRotation(), SpawnParams);
	Temp->OnCreation();
	
	ABlockDefenseGameMode* GM = Cast<ABlockDefenseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->ReplaceTile(Temp);
	GM->UnlockAdjacentTiles(Temp);
	GM->TriggerTileUpdateVisualEvent(Temp->GetActorLocation(), 1.0f);
	Destroy();

}


bool AMap_Tile_Game::CanTileTransform(const int Currency)
{
	bTransformConditionsMet = (Currency >= TransformationCost && (!bFixedState && !bLockedState));
	return bTransformConditionsMet;
}
