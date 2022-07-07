// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/Buildables/ConstructionBlock.h"
#include "BlockDefense/BlockDefensePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "BlockDefense/Map/Map_ConstructionGrid.h"

// Sets default values
AConstructionBlock::AConstructionBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

}

// Called when the game starts or when spawned
void AConstructionBlock::BeginPlay()
{
	Super::BeginPlay();
	

	
	
}

// Called every frame
void AConstructionBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

void AConstructionBlock::RemoveFromGrid()
{
	FVector Temp = FVector((int)GetActorLocation().X, (int)GetActorLocation().Y, (int)GetActorLocation().Z) / Grid->GetTileScale();

	Grid->RemoveBlock(Temp);
	

}


