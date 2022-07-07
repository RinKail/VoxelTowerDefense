// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/Map/Map_Tile_Editor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "BlockDefense/Map/Map_Tile_Game.h"
#include "Engine/World.h"



AMap_Tile_Editor::AMap_Tile_Editor()
{
	Mode = ETileMode::TM_None;
}

void AMap_Tile_Editor::OnConstruction(const FTransform& Transform)
{

	UpdateTile(TransformTo);


}



AMap_Tile_Game* AMap_Tile_Editor::GenerateGameplayActor()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	switch (TransformTo)
	{
	case ETileMode::TM_Resource:
		return GetWorld()->SpawnActor<AMap_Tile_Game>(ResourceClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		
	case ETileMode::TM_Platform:
		return GetWorld()->SpawnActor<AMap_Tile_Game>(PlatformClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		
	case ETileMode::TM_Buildable:
		return GetWorld()->SpawnActor<AMap_Tile_Game>(BuildableClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		
	case ETileMode::TM_Passage:
		return GetWorld()->SpawnActor<AMap_Tile_Game>(PassageClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		
	}

	return nullptr;
}


void AMap_Tile_Editor::UpdateTile(ETileMode NewMode)
{
	

	switch (NewMode)
	{
	case ETileMode::TM_Resource:
		MeshComponent->SetStaticMesh(ResourceMesh);
		break;
	case ETileMode::TM_Platform:
		MeshComponent->SetStaticMesh(PlatformMesh);
		break;
	case ETileMode::TM_Buildable:
		MeshComponent->SetStaticMesh(BuildableMesh);
		break;
	case ETileMode::TM_Passage:
		MeshComponent->SetStaticMesh(PassageMesh);
		break;
	}
	



}

