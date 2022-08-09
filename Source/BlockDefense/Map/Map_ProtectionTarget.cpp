// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/Map/Map_ProtectionTarget.h"
#include "BlockDefense/BlockDefenseGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMap_ProtectionTarget::AMap_ProtectionTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMap_ProtectionTarget::BeginPlay()
{
	Super::BeginPlay();
	Health = BaseHealth;
	
}

// Called every frame
void AMap_ProtectionTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMap_ProtectionTarget::DamageCore(const int Damage, class AEnemyBase* Unit)
{
	Health -= Damage;
	 OnDamageTaken(Damage);
	 ABlockDefenseGameMode* GM = Cast<ABlockDefenseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	 GM->UnitDestroyed(Unit);
	if (Health <= 0)
	{
	
		GM->OnGameOver();
	}
}

