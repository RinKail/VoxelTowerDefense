// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/Buildables/Buildable_ItemCursor.h"

// Sets default values
ABuildable_ItemCursor::ABuildable_ItemCursor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildable_ItemCursor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildable_ItemCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

