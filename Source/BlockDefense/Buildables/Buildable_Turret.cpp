// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/Buildables/Buildable_Turret.h"
#include "BlockDefense/Buildables/Buildable_Structure.h"
#include "BlockDefense/BlockDefenseGameMode.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


ABuildable_Turret::ABuildable_Turret()
{
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Cannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon"));

	FAttachmentTransformRules Temp = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);

	
	SetRootComponent(Base);
	Cannon->SetupAttachment(Base);
	bIsTurret = true;
	Target = nullptr;
	bHasTarget = false;

	BonusDamage = 1.0f;
	BonusRange = 0;
	BonusShots = 1.0f;

	
}

void ABuildable_Turret::BeginPlay()
{

	GameModeReference = Cast<ABlockDefenseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateTarget, this, &ABuildable_Turret::UpdateTarget, TargetUpdateFrequency, true);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Fire, this, &ABuildable_Turret::Fire, (float)1 / (ShotsPerSecond * BonusShots), true);
}

void ABuildable_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bHasTarget && IsValid(Target))
	{
		
		FRotator YawRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
		FRotator PitchRot = UKismetMathLibrary::FindLookAtRotation(Cannon->GetComponentLocation(), Target->GetActorLocation());
		SetActorRotation(FRotator(0, YawRot.Yaw, 0));
		Cannon->SetWorldRotation(FRotator(Cannon->GetComponentRotation().Pitch, Cannon->GetComponentRotation().Yaw, PitchRot.Roll));
	}
}

void ABuildable_Turret::Upgrade(EBlockType Type)
{
}

void ABuildable_Turret::RoundStart()
{

	UpdateTarget();
	OnRoundStart();

	
}

void ABuildable_Turret::RoundEnd()
{
	
	OnRoundEnd();
}

void ABuildable_Turret::ClearStructureLinks()
{
	LinkedBuildings.Empty();
	BonusDamage = 1.0f;
	BonusRange = 0;
	BonusShots = 1.0f;


}
void ABuildable_Turret::AddStructureLink(class ABuildable_Structure* NewLink)
{
	LinkedBuildings.Add(NewLink);
	UpdateBonusStats();
	NewLink->OnLink(this);
}

void ABuildable_Turret::RemoveStructureLink(class ABuildable_Structure* TargetLink)
{
	LinkedBuildings.Remove(TargetLink);
	BonusDamage -= TargetLink->GetBonusDamage();
	BonusRange -= TargetLink->GetBonusRange();
	BonusShots -= TargetLink->GetBonusRange();

}
void ABuildable_Turret::UpdateBonusStats()
{
	BonusDamage = 1.0f;
	BonusRange = 0;
	BonusShots = 1.0f;
	for (int i = 0; i < LinkedBuildings.Num(); i++)
	{
		BonusDamage += LinkedBuildings[i]->GetBonusDamage();
		BonusRange += LinkedBuildings[i]->GetBonusRange();
		BonusShots += LinkedBuildings[i]->GetBonusRange();
	}
}

void ABuildable_Turret::Fire()
{	
	if (bHasTarget && IsValid(Target))
	{

		if (Target != nullptr) OnHit(Target);
		for (int i = 0; i < LinkedBuildings.Num(); i++)
		{
			if (Target != nullptr) LinkedBuildings[i]->OnDamageUnit(this, Target);
		}
		if (Target != nullptr) Target->DamageUnit(Damage * BonusDamage, DamageProperty, this);
		
	}
	
}


void ABuildable_Turret::UpdateTarget()
{
	bHasTarget = false;
	TArray<AEnemyBase*> UnitList;
	Target = nullptr;

	
	for (int i = 0; i < GameModeReference->GetEnemyList().Num(); i++)
	{
		if (PriorityType == GameModeReference->GetEnemyList()[i]->GetBodyType() && FVector::Distance(GetActorLocation(), GameModeReference->GetEnemyList()[i]->GetActorLocation()) <= Range + BonusRange)
		{
			UnitList.Add(GameModeReference->GetEnemyList()[i]);
		}
	}
	if (UnitList.Num() == 0)
	{
		for (int i = 0; i < GameModeReference->GetEnemyList().Num(); i++)
		{
			if (FVector::Distance(GetActorLocation(), GameModeReference->GetEnemyList()[i]->GetActorLocation()) <= Range + BonusRange)
			{
				UnitList.Add(GameModeReference->GetEnemyList()[i]);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Unit List Length: %d"), UnitList.Num());
	float MaxDistance = 99999999999999999.0f;
	float MinDistance = 0.0f;
	int MaxOrder = 999;
	int MinOrder = 0;
	float MinThreat = 0;

	

	for (int i = 0; i < UnitList.Num(); i++)
	{

		
			if (IsValid(UnitList[i]))
			{
				switch (PriorityPosition)
				{
				case ETargetingMode::TM_Furthest:
					if (FVector::Distance(GetActorLocation(), UnitList[i]->GetActorLocation()) > MinDistance)
					{
						MinDistance = FVector::Distance(GetActorLocation(), UnitList[i]->GetActorLocation());
						Target = UnitList[i];
					}
					break;
				case ETargetingMode::TM_Nearest:
					if (FVector::Distance(GetActorLocation(), UnitList[i]->GetActorLocation()) < MaxDistance)
					{
						MaxDistance = FVector::Distance(GetActorLocation(), UnitList[i]->GetActorLocation());
						Target = UnitList[i];
					}
					break;
				case ETargetingMode::TM_First:
					if (UnitList[i]->UnitOrder() > MinOrder)
					{
						MinOrder = UnitList[i]->UnitOrder();
						Target = UnitList[i];
					}
					break;
				case ETargetingMode::TM_Last:
					if (UnitList[i]->UnitOrder() < MaxOrder)
					{
						MaxOrder = UnitList[i]->UnitOrder();
						Target = UnitList[i];
					}
					break;
				case ETargetingMode::TM_Strongest:
					if (UnitList[i]->GetThreatLevel() > MinThreat)
					{
						MinThreat = UnitList[i]->GetThreatLevel();
						Target = UnitList[i];
					}
					break;
				}
			}
	}
	bHasTarget = IsValid(Target);
	
}
