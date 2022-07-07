// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockDefense/Buildables/BuildableBase.h"
#include "BlockDefense/AI/EnemyBase.h"
#include "BlockDefense/Map/Map_ConstructionGrid.h"
#include "Buildable_Turret.generated.h"



UENUM(BlueprintType)
enum class ETargetingMode : uint8
{
	TM_Furthest		 UMETA(DisplayName = "Furthest"), //Prioritise the furthest enemy unit
	TM_Nearest		 UMETA(DisplayName = "Nearest"), //Prioritise the closest enemy unit

	TM_First		 UMETA(DisplayName = "First"), //prioritise the unit furthest along the path.
	TM_Last		   UMETA(DisplayName = "Last"),	//Prioritise the unit the least furthest along the path.
	TM_Strongest	UMETA(DisplayName = "Strongest"),

};





/**
 * 
 */
UCLASS()
class BLOCKDEFENSE_API ABuildable_Turret : public ABuildableBase
{
	GENERATED_BODY()


protected: 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* Base;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* Cannon;


public:

	ABuildable_Turret();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintImplementableEvent)
	void OnCreated();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpgraded();
	UFUNCTION(BlueprintImplementableEvent)
	void OnRoundStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnRoundEnd();
	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(class AEnemyBase* HitUnit);


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Targeting")
	class AEnemyBase* GetTarget() { return Target; }


	virtual void Upgrade(EBlockType Type);

	void RoundStart();

	void RoundEnd();



	void ClearStructureLinks();
	void UpdateBonusStats();
	void AddStructureLink(class ABuildable_Structure* NewLink);
	void RemoveStructureLink(class ABuildable_Structure* TargetLink);

	void ClearTarget() {
		bHasTarget = false;
		Target = nullptr;
	}

protected:
	//How many projectiles are fired per second/
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,Category = "Attack")
	float ShotsPerSecond;
	//How much range does the turret have, this is multiplied by the TileScale.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	int Range;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		EBlockType DamageProperty;



	
	//Bonus properties utilised by buildings.

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float BonusShots;
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	int BonusRange;
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float BonusDamage;



	//Uint position to prioritise
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting")
	ETargetingMode PriorityPosition;
	//Which unit type to prioritise
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting")
	EBlockType	PriorityType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting")
		float TargetUpdateFrequency;


	bool bHasTarget;

	void Fire();
	

	void UpdateTarget();

	class AEnemyBase* Target;
	



	TArray<class ABuildable_Structure*> LinkedBuildings;

private: 
	FTimerHandle TimerHandle_UpdateTarget;
	FTimerHandle TimerHandle_Fire;

	class ABlockDefenseGameMode* GameModeReference;

	float FireRate;


};
