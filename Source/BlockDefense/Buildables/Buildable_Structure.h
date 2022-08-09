// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockDefense/Buildables/BuildableBase.h"
#include "Buildable_Structure.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKDEFENSE_API ABuildable_Structure : public ABuildableBase
{
	GENERATED_BODY()
public:
		ABuildable_Structure();


		virtual void Constructed(class AMap_ConstructionGrid* InGrid);


		void LinkTurret(class ABuildable_Turret* Turret);
		void UnlinkTurrets();

		void AddRecipes();

		float GetBonusDamage() { return BonusDamage; }
		int GetBonusShots() { return BonusShots; }
		int GetBonusRange() { return BonusRange; }

protected: 

		UPROPERTY(EditDefaultsOnly, Category = "Unlockables")
		TArray<FName> UnlockRecipes;

		//How big the range is, in grid spaces.
		UPROPERTY(EditAnywhere, Category = "BonusStats")
		int LinkRange;

		//Additional Damage to add to towers
		UPROPERTY(EditAnywhere, Category = "BonusStats")
		float BonusDamage;
		//Additional Fire rate to add to towers
		UPROPERTY(EditAnywhere, Category = "BonusStats")
		int BonusShots;
		//Additioanl Range to add to Towers.
		UPROPERTY(EditAnywhere, Category = "BonusStats")
		int BonusRange;

		UFUNCTION(BlueprintCallable, Category = "Player")
		void IncreasePlayerPoints(const int Value);

public:

		//Called when a turret links to this structure
		UFUNCTION(BlueprintImplementableEvent)
		void OnLink(class ABuildable_Turret* LinkedTurret);

		//Called whenever a turret linked to this structure deals damage.
		//Provides the source of the event and the target hit.
		UFUNCTION(BlueprintImplementableEvent)
		void OnDamageUnit(class ABuildable_Turret* LinkedTurret, class AEnemyBase* HitTarget);
	
};
