// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/Map/Map_ChunkBuilder.h"
#include "BlockDefense/Map/Map_Tile.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"

// Sets default values
AMap_ChunkBuilder::AMap_ChunkBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Main = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Main);
}

// Called when the game starts or when spawned
void AMap_ChunkBuilder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMap_ChunkBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AMap_ChunkBuilder::GenerateChunk()
{
	for (int x = 0; x < ChunkScale; x++)
	{
		for (int y = 0; y < ChunkScale; y++)
		{
			if (GetWorld() != nullptr)
			{
				AMap_Tile* TempTile = GetWorld()->SpawnActor<AMap_Tile>(TileClass);
				TempTile->SetActorLocation(FVector(GetActorLocation().X + (x * TileScale), GetActorLocation().Y + (y * TileScale), 0.0f));
			}

		}
	}
}

