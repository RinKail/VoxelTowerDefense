// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlockDefensePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BlockDefense/Map/Map_Tile_Game.h"
#include "BlockDefense/Buildables/ConstructionBlock.h"
#include "BlockDefense/Map/Map_ConstructionGrid.h"
#include "BlockDefense/UI/PlayerHudBase.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "BlockDefenseCharacter.h"
#include "Engine/World.h"

ABlockDefensePlayerController::ABlockDefensePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	InitialMouseLocation = FVector(0, 0, 0);
	bInputPressed = false;
	
	ConstructionGrid = nullptr;
	SelectedTile = nullptr;
	SelectedStructure = nullptr;
	WidgetInspectWindow = nullptr;
	WidgetResourceBar = nullptr;
	bIsGridOnCooldown = false;
	GridCooldown = 1.5f;
	Currency = 0;
	CurrentScore = 0;
	ConstructionCost = 500;
}


void ABlockDefensePlayerController::BeginPlay()
{
	
	
	if (IsValid(ResourceBarClass) && IsValid(GridClass) && IsValid(InspectWindowClass))
	{
		int TempCurrency = Currency;
		Currency = 0;
		WidgetResourceBar = CreateWidget<UResourceBar>(GetWorld(), ResourceBarClass, FName(TEXT("ResourceBar")));
		WidgetResourceBar->AddToViewport();
		ConstructionGrid = GetWorld()->SpawnActor<AMap_ConstructionGrid>(GridClass);
		WidgetInspectWindow = CreateWidget<UInspectWindow>(GetWorld(), InspectWindowClass, FName(TEXT("InspectWindow")));
		WidgetInspectWindow->AddToViewport();
		IncreasePoints(TempCurrency);
		APawn* const MyPawn = GetPawn();
		if (MyPawn) CameraStartLocation = MyPawn->GetActorLocation();

		for (int i = 0; i < UnlockRecipes.Num(); i++)
		{
			ConstructionGrid->AddAvailableStructure(UnlockRecipes[i]);
		}
	}
	

	Super::BeginPlay();

}

void ABlockDefensePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();


	InputComponent->BindAction("ApplyTileUpdate", IE_Released, this, &ABlockDefensePlayerController::ApplyTileUpdate);

	InputComponent->BindAction("CreateCube", IE_Released, this, &ABlockDefensePlayerController::CreateConstructionBlock);

	InputComponent->BindAction("SetConstruct_Red", IE_Released, this, &ABlockDefensePlayerController::SetConstructionType_Red);
	InputComponent->BindAction("SetConstruct_Green", IE_Released, this, &ABlockDefensePlayerController::SetConstructionType_Green);
	InputComponent->BindAction("SetConstruct_Blue", IE_Released, this, &ABlockDefensePlayerController::SetConstructionType_Blue);
	InputComponent->BindAction("SetConstruct_None", IE_Released, this, &ABlockDefensePlayerController::SetConstructionType_None);



	InputComponent->BindAction("MoveCamera", IE_Pressed, this, &ABlockDefensePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("MoveCamera", IE_Released, this, &ABlockDefensePlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ABlockDefensePlayerController::OnTouchPressed);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &ABlockDefensePlayerController::OnTouchReleased);

}



bool ABlockDefensePlayerController::CameraInRange()
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		return (FVector2D::Distance(FVector2D(CameraStartLocation), FVector2D(InitialMouseLocation)) < MovementRange);
	}
	else return false;
	
}
void ABlockDefensePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);


	



	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		//Camera movement, if the camera is in range and the input button is pressed, move the camera
		if (bInputPressed && CameraInRange())
		{



			FVector HitLocation = FVector::ZeroVector;
			FHitResult Hit;
			//Gets a floor hit, only looks for the grid placed bellow the map tiles.
			GetHitResultUnderCursor(ECC_TRACE_GROUND, true, Hit);
			HitLocation = Hit.Location;
			//Get the offset between the initial position and the current position
			FVector Dist = (InitialMouseLocation - HitLocation);
			Dist.Z = 0;
			
			//Offset the controlled pawn's position by the calulated distance
			MyPawn->SetActorLocation(MyPawn->GetActorLocation() + (Dist * DeltaTime));

		}
		//Trace to the floor again, this time looking for a tile.
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_TRACE_TILE, true, Hit);
		if (Hit.bBlockingHit)
		{
			//If hit tile is not the currently cached tile, perform logic
			//Saves resources and makes sure these events are called at the correct time.
			if (Hit.GetActor() != SelectedTile)
			{
				//If the previous tile actually existed, tell it to end it's highlighted state.
				if (SelectedTile != nullptr) SelectedTile->OnHighlightEnd();
				//Cast the found tile to the Tile class and store it under the selected tile, replacing the previous reference.
				SelectedTile = Cast<AMap_Tile_Game>(Hit.GetActor());
				//Can the tile be modified? Updates the tile so that the cursor can display if it can currently be changed.
				SelectedTile->CanTileTransform(Currency);

				//Do we have a selected structure?
				//Since this is being called, the tile has changed and as such tnat means that any exisint structure reference is from a different tile, end the selection/Highlight.
				if (SelectedStructure != nullptr)
				{
					SelectedStructure->OnSelectEnd();
				
					WidgetInspectWindow->ItemHoverEnd();
				}
				
			
				//Safety check, then get the buildable at the tile location and store it. Buildables account for ConstructionBlocks, Turrets and Towers.
				if (ConstructionGrid != nullptr) SelectedStructure = ConstructionGrid->GetBuidableAt(SelectedTile->GetActorLocation());

				//Did we find a buildable? Trigger highlight and selection code for only the buildable, otherwise, trigger the highlight code for the foundation tile.
				if (SelectedStructure != nullptr)
				{
					SelectedStructure->OnSelectStart();
					WidgetInspectWindow->ItemHoverStart(SelectedStructure->GetItemID());
				}
				else
				{
					SelectedTile->OnHighlightStart();
				}

			}

		}

	}

	


}

void ABlockDefensePlayerController::ApplyTileUpdate()
{
	//Makes sure the tile exists, then checks if there are transforms slots remaining. 
	//Is the tile affordable? 
	//Is there no structure placed ontop of it?
	//Is the placement mode null, meaning no construction block.
	if ((SelectedTile != nullptr) && (AvailableTransformSlots > 0 && SelectedTile->CanTileTransform(Currency)) && SelectedStructure == nullptr && WidgetResourceBar->GetSelectedType() == EBlockType::BT_None)
	{
		//Transforms the tile.
		SelectedTile->TransformTile(this);
		bIsGridOnCooldown = true;
		//Put the tile on cooldown
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_GridCooldown, this, &ABlockDefensePlayerController::ReleaseGridCooldown, GridCooldown);
		//Transform the tile based on it's cost. 
		//Different tile types have different costs. 
		//Resource blocks cost a high amount due to unlocking additional build space, platforms are cheap as they are required for building structures, removing a buildable will create an AI passage, changing the path the AI might take.
		SpendPoints(SelectedTile->GetCost());

	}

	else
	{
		//WidgetInspectWindow->ItemUnselected();
	}


}

void ABlockDefensePlayerController::CreateConstructionBlock()
{
	//Checks if the current tile is buildable, also makes sure the selected type is not None. Do we have enough money to place a block?
	if (SelectedTile != nullptr)
	{
		if (SelectedTile->CanBuild() && WidgetResourceBar->GetSelectedType() != EBlockType::BT_None && Currency >= ConstructionCost)
		{
			//If there was a selected structure, stop selecting it.
			if (SelectedStructure != nullptr) SelectedStructure->OnSelectEnd();
			//Goes to the construction grid and gives it the current position of interest. Informs it of the type and whether it's being placed on a root.
			SelectedStructure = ConstructionGrid->AddBlock(SelectedTile->GetTileLocation() + FVector(0, 0, SelectedTile->GetTileScale()), WidgetResourceBar->GetSelectedType(), SelectedTile->IsBuildableTile());
			//Start selecting the newly made Buildable.
			SelectedStructure->OnSelectStart();
			WidgetInspectWindow->ItemHoverStart(SelectedStructure->GetItemID());
			//Put the grid on a cooldown then spend the cost of placing a block.
			bIsGridOnCooldown = true;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_GridCooldown, this, &ABlockDefensePlayerController::ReleaseGridCooldown, GridCooldown);
			SpendPoints(500);	//ToDo, Update the code to read the cost from the data table instead of being hard coded.
		}
	}
	
	
}

//Input binds that update the selected type.
//Stored inside the UI instead of here to remove the requirement for two way references.


void ABlockDefensePlayerController::SetConstructionType_Red()
{
	WidgetResourceBar->SetSelectedType(EBlockType::BT_Red);
}
void ABlockDefensePlayerController::SetConstructionType_Green()
{
	WidgetResourceBar->SetSelectedType(EBlockType::BT_Green);
}
void ABlockDefensePlayerController::SetConstructionType_Blue()
{
	WidgetResourceBar->SetSelectedType(EBlockType::BT_Blue);
}
void ABlockDefensePlayerController::SetConstructionType_None()
{
	WidgetResourceBar->SetSelectedType(EBlockType::BT_None);
}





void ABlockDefensePlayerController::OnSetDestinationPressed()
{
	// We flag that the input is being pressed
	bInputPressed = true;

	FVector HitLocation = FVector::ZeroVector;
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_TRACE_GROUND, true, Hit);
	InitialMouseLocation = Hit.Location;

	//When right clicking on structure, select it.

	 if (SelectedStructure != nullptr)
	{
		WidgetInspectWindow->NewItemSelected(SelectedStructure->GetItemID(), SelectedStructure);
	}


	StopMovement();
}

void ABlockDefensePlayerController::OnSetDestinationReleased()
{
	// Player is no longer pressing the input
	bInputPressed = false;

	
}

void ABlockDefensePlayerController::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bIsTouch = true;
	OnSetDestinationPressed();
}

void ABlockDefensePlayerController::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bIsTouch = false;
	OnSetDestinationReleased();
}


void ABlockDefensePlayerController::IncreaseScore(int Increase)
{
	Currency += Increase;
	CurrentScore += Increase;
	WidgetResourceBar->PointsIncreased(Currency);
}
void ABlockDefensePlayerController::IncreasePoints(int Increase)
{
	Currency += Increase;
	WidgetResourceBar->PointsIncreased(Increase);
}
void ABlockDefensePlayerController::SpendPoints(int Decrease)
{
	Currency -= Decrease;
	WidgetResourceBar->PointsDecreased(Decrease);
}

