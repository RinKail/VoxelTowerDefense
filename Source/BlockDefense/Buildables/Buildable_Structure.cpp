// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDefense/Buildables/Buildable_Structure.h"
#include "BlockDefense/Buildables/Buildable_Turret.h"
#include "BlockDefense/Map/Map_ConstructionGrid.h"
#include "BlockDefense/BlockDefensePlayerController.h"
#include "Kismet/GameplayStatics.h"

ABuildable_Structure::ABuildable_Structure()
{
	bIsTurret = false;
}

void ABuildable_Structure::Constructed(class AMap_ConstructionGrid* InGrid)
{
	Grid = InGrid;

	TArray<class ABuildableBase*> TurretsInRange = Grid->GetStructuresInRange(LinkRange, this, true);
	for (int i = 0; i < TurretsInRange.Num(); i++)
	{
		Cast<ABuildable_Turret>(TurretsInRange[i])->AddStructureLink(this);
	}

	AddRecipes();
	OnBuild();
}
void ABuildable_Structure::IncreasePlayerPoints(int Value)
{
	Cast<ABlockDefensePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->IncreasePoints(Value);
}
void ABuildable_Structure::LinkTurret(ABuildable_Turret* Turret)
{
	OnLink(Turret);
	Turret->UpdateBonusStats();
}
void ABuildable_Structure::UnlinkTurrets()
{

}
void ABuildable_Structure::AddRecipes()
{
	for (int i = 0; i < UnlockRecipes.Num(); i++)
	{
		Grid->AddAvailableStructure(UnlockRecipes[i]);
	}
}
