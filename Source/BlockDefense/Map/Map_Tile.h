// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Map_Tile.generated.h"


UENUM(BlueprintType)
enum class ETileMode : uint8
{
	TM_None      UMETA(DisplayName = "None"),
	TM_Resource      UMETA(DisplayName = "Resource"),
	
	TM_Platform      UMETA(DisplayName = "Platform"),
	TM_Buildable	 UMETA(DisplayName = "Buildable"),
	TM_Passage		 UMETA(DisplayName = "Passage"),
};


UCLASS(hidecategories = (Physics, Rendering, Replication, Input, Actor, Shape))
class BLOCKDEFENSE_API AMap_Tile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMap_Tile();


	//Triggered when a grid tile is changed. 
	//Should be used for any visual events.
	UFUNCTION(BlueprintImplementableEvent)
		void OnTileChanged(FVector TilePosition, float Intensity);




	//Getters
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TileState")
	bool IsTileLocked() { return bLockedState; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TileState")
	ETileMode GetTileMode() { return Mode; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TileState")
	int GetClusterChannel() { return ClusterChannel; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TileState")
	int GetTileScale() { return TileScale; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TileState")
	FVector GetTileLocation() { return FVector((int)GetActorLocation().X, (int)GetActorLocation().Y, 0); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TileState")
	bool IsBuildableTile() { return Mode == ETileMode::TM_Buildable; }

	bool CanBuild() { return (Mode == ETileMode::TM_Buildable || Mode == ETileMode::TM_Platform); }



protected:

//Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* MeshComponent;

	


	
	


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform);

	//The hard coded mode, this is set in the child classes.
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly, Category = "Tile Gameplay")
	ETileMode Mode;

	//Determines what type of tile to transform into.
	UPROPERTY(EditAnywhere, Category = "Tile Transformation")
	ETileMode TransformTo;
	
	

	//Is this tile's mode currently locked?
	UPROPERTY(EditAnywhere, Category = "Tile Gameplay")
	bool bLockedState;
	//Can this tile's mode change during gameplay?
	UPROPERTY(EditAnywhere, Category = "Tile Gameplay")
	bool bFixedState;

	//Cluster channel, when a clustered tile is broken, all tile's sharing it's cluster will also break. Non-Cluster Tiles should be set to 0.
	UPROPERTY(EditAnywhere, Category = "Tile Gameplay")
	int ClusterChannel;

	





	


	
	


	//The scale at which a tile's height can be shifted by the system.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile Visuals")
	float TileHeightVariance;

	//How large the tile is in world space, this is used to establish grid network.
	UPROPERTY(EditAnywhere, Category = "Tile Visuals")
	float TileScale;

	

	UPROPERTY();
	class ABlockDefensePlayerController* ControllerReference;

public:	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHighlightStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnHighlightEnd();




	// Called every frame
	virtual void Tick(float DeltaTime) override;

	




	virtual void TransformTile(ABlockDefensePlayerController* Controller);
	virtual void TransformTile_Complete();


	void SetLockState(bool bNewState) { bLockedState = bNewState; }



	
	





};
