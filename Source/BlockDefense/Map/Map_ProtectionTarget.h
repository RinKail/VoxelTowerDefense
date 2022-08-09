// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Map_ProtectionTarget.generated.h"

UCLASS()
class BLOCKDEFENSE_API AMap_ProtectionTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMap_ProtectionTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable, Category = "Game")
	void DamageCore(int Damage, class AEnemyBase* Unit);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game")
	float HealthPercentage() {return  Health / BaseHealth; }

protected: 

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTaken(const int DamageTaken);

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float BaseHealth;





	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
