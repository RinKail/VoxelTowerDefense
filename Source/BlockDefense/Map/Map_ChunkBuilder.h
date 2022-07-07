// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Map_ChunkBuilder.generated.h"

UCLASS()
class BLOCKDEFENSE_API AMap_ChunkBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMap_ChunkBuilder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class USceneComponent* Main;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(CallInEditor, Category = "Chunk")
	void GenerateChunk();

	UPROPERTY(EditAnywhere, Category = "Chunk")
	int ChunkScale;

	UPROPERTY(EditAnywhere, Category = "Chunk")
		int TileScale;

	UPROPERTY(EditAnywhere, Category = "Chunk")
	TSubclassOf<class AMap_Tile> TileClass;

};
