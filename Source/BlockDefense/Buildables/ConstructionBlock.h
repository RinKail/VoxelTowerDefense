// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockDefense/Buildables/BuildableBase.h"
#include "ConstructionBlock.generated.h"







UCLASS()
class BLOCKDEFENSE_API AConstructionBlock : public ABuildableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConstructionBlock();

	void SetType(EBlockType Type) {
		BlockType = Type;
		OnTypeChanged(Type);
	}
	
protected: 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComponent;

	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Block")
	EBlockType BlockType;

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	float BlockSize;

	virtual void RemoveFromGrid() override;


	UFUNCTION(BlueprintImplementableEvent)
	void OnTypeChanged(EBlockType Type);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
