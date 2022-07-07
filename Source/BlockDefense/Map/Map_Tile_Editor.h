// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockDefense/Map/Map_Tile.h"
#include "Map_Tile_Editor.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKDEFENSE_API AMap_Tile_Editor : public AMap_Tile
{
	GENERATED_BODY()




public:


	AMap_Tile_Editor();

	class AMap_Tile_Game* GenerateGameplayActor();

	virtual void OnConstruction(const FTransform& Transform);


protected: 


	


	//What this tile is going to turn into.
	UPROPERTY(EditAnywhere, Category = "Tile Meshes")
		class UStaticMesh* ResourceMesh;

	//What this tile is going to turn into.
	UPROPERTY(EditAnywhere, Category = "Tile Meshes")
		class UStaticMesh* PlatformMesh;

	//What this tile is going to turn into.
	UPROPERTY(EditAnywhere, Category = "Tile Meshes")
		class UStaticMesh* BuildableMesh;

	//What this tile is going to turn into.
	UPROPERTY(EditAnywhere, Category = "Tile Meshes")
		class UStaticMesh* PassageMesh;



	//What this tile is going to turn into.
	UPROPERTY(EditAnywhere, Category = "Tile Classes")
		TSubclassOf<class AMap_Tile_Game> ResourceClass;

	//What this tile is going to turn into.
	UPROPERTY(EditAnywhere, Category = "Tile Classes")
		TSubclassOf<class AMap_Tile_Game> PlatformClass;

	//What this tile is going to turn into.
	UPROPERTY(EditAnywhere, Category = "Tile Classes")
		TSubclassOf<class AMap_Tile_Game> BuildableClass;

	//What this tile is going to turn into.
	UPROPERTY(EditAnywhere, Category = "Tile Classes")
		TSubclassOf<class AMap_Tile_Game> PassageClass;


	

	//Update this tile's state.

		void UpdateTile(ETileMode NewMode);

};
