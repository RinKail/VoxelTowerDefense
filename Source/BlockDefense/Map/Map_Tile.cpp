// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/Map/Map_Tile.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "BlockDefense/BlockDefensePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"

// Sets default values
AMap_Tile::AMap_Tile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	SetRootComponent(MeshComponent);


	TileScale = 200;


}

// Called when the game starts or when spawned
void AMap_Tile::BeginPlay()
{



	Super::BeginPlay();


	
}





void AMap_Tile::OnConstruction(const FTransform& Transform)
{

	//UpdateTile(Mode);

	
}

// Called every frame
void AMap_Tile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}





void AMap_Tile::TransformTile(ABlockDefensePlayerController* Controller)
{
	ControllerReference = Controller;
	ControllerReference->TransformTile_Start();

}

void AMap_Tile::TransformTile_Complete()
{
	ControllerReference->TransformTile_End();
	//UpdateTile(TransformTo);
	ControllerReference = nullptr;
}
