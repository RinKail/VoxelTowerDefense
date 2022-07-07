// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/AI/EnemyBase.h"
#include "BlockDefense/BlockDefenseGameMode.h"
#include "BlockDefense/Buildables/BuildableBase.h"
#include "BlockDefense/Buildables/Buildable_Turret.h"
#include "BlockDefense/BlockDefensePlayerController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::Destroyed()
{


	


	Super::Destroyed();
	
}

void AEnemyBase::DamageUnit(float Damage, EBlockType DamageType,  ABuildableBase* DamageSource)
{
	UE_LOG(LogTemp, Warning, TEXT("Units Remaining: %s"), *DamageSource->GetName());
	OnDamageTaken();

	

	if (DamageType == BodyType) Health -= Damage;
	else
	{
		Health -= Damage * 0.25;
	}
	

	if (Health <= 0)
	{
		Cast<ABlockDefenseGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->UnitDestroyed(this);
		if (DamageSource->IsTurret())
		{
			Cast<ABuildable_Turret>(DamageSource)->ClearTarget();
			Cast<ABlockDefensePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->IncreasePoints(PointsWorth);
		}

		
	}

}

void AEnemyBase::BodyDamage(float Damage, float BlueMod, float RedMod, float GreenMod)
{
	switch (BodyType)
	{
	case EBlockType::BT_Blue:
		Health -= Damage * BlueMod;
		break;
	case EBlockType::BT_Red:
		Health -= Damage * RedMod;
		break;
	case EBlockType::BT_Green:
		Health -= Damage * GreenMod;
		break;
	default: 
		Health -= Damage;
		break;
	}
}


void AEnemyBase::SetBodyType(EBlockType NewType)
{
	BodyType = NewType;
	OnTypeSet(NewType);
}


void AEnemyBase::InitialiseValues(int CurrentWave)
{
	Health += CurrentWave * 5;
	Speed += CurrentWave * 5;
	MovementComponent->MaxSpeed = Speed;
}
