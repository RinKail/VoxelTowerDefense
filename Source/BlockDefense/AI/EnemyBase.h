// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BlockDefense/Map/Map_ConstructionGrid.h"
#include "EnemyBase.generated.h"




UENUM(BlueprintType)
enum class EBodyType : uint8
{

	BT_Normal		 UMETA(DisplayName = "Normal"),
	BT_Armour		UMETA(DisplayName = "Armour"),
	BT_Shield		 UMETA(DisplayName = "Shield"),

	BT_Cloaked		UMETA(DisplayName = "Cloaked"),

};

UENUM(BlueprintType)
enum class EDamageType : uint8
{


	DT_Kinetic		UMETA(DisplayName = "Kinetic"),
	DT_Piercing		 UMETA(DisplayName = "Piercing"),
	DT_Energy		 UMETA(DisplayName = "Energy"),
	

};



UCLASS()
class BLOCKDEFENSE_API AEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBase();


	EBlockType GetBodyType() { return BodyType; }
	int UnitOrder() { return Order; }
	int GetThreatLevel() { return ThreatLevel; }




	void SetBodyType(EBlockType NewType);

	virtual void Destroyed() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UFloatingPawnMovement* MovementComponent;


	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTaken();
	UFUNCTION(BlueprintImplementableEvent)
	void OnTypeSet(EBlockType Type);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	int Order;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	int ThreatLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	EBlockType BodyType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Shields;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Armour;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Points")
	int PointsWorth;



	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitialiseValues(int CurrentWave);

	void DamageUnit(float Damage, EBlockType DamageType, class ABuildableBase* DamageSource);

	void BodyDamage(float Damage, float ArmourMod, float ShieldMod, float NormalMod);

	
private: 

	//Stored reference to the Gamemode, this is only needed on unit destruction.
	UPROPERTY()
	class ABlockDefenseGameMode* GameModeReference;
};
