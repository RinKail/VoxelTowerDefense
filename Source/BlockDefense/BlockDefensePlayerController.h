// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "BlockDefensePlayerController.generated.h"

#define ECC_TRACE_TILE ECC_GameTraceChannel1
#define ECC_TRACE_GROUND ECC_GameTraceChannel2

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ABlockDefensePlayerController : public APlayerController
{
	GENERATED_BODY()

public:


	ABlockDefensePlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;
	//Delay between being able to modify the grid again.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float GridCooldown;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;


	UFUNCTION(BlueprintCallable)
	void SetCameraLock(bool bCanCameraMove) { bLockedCamera = !bCanCameraMove; }

	virtual void BeginPlay() override;


	void TransformTile_Start() { AvailableTransformSlots--; }
	void TransformTile_End() { AvailableTransformSlots++; }

	
	void IncreaseScore(int Increase);
	void IncreasePoints(int Increase);
	void SpendPoints(int Decrease);

	int GetScore() { return CurrentScore; }

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	void OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementRange;

	UPROPERTY()
	FVector CameraStartLocation;

	//Can the player move the camera?
	UPROPERTY(EditAnywhere, Category = "Camera")
		bool bLockedCamera;


	

	
	//The construction block class assigned to this controller. Is set through blueprints to avoid hard references.
	UPROPERTY(EditAnywhere, Category = "Classes")
	TSubclassOf<class AMap_ConstructionGrid> GridClass;

	UPROPERTY(EditDefaultsOnly, Category = "Classes")
		TSubclassOf<class UInspectWindow> InspectWindowClass;
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
		TSubclassOf<class UResourceBar> ResourceBarClass;


	//
	UPROPERTY()
	class AMap_Tile_Game* SelectedTile;
	UPROPERTY()
	class UInspectWindow* WidgetInspectWindow;
	UPROPERTY()
	class UResourceBar* WidgetResourceBar;


	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int CurrentScore;
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int Currency;
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int AvailableTransformSlots;


	void ApplyTileUpdate();

	void CreateConstructionBlock();

	void SetConstructionType_None();
	void SetConstructionType_Red();
	void SetConstructionType_Green();
	void SetConstructionType_Blue();

	UPROPERTY(EditDefaultsOnly, Category = "Unlockables")
		TArray<FName> UnlockRecipes;
	

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int ConstructionCost;

private:

	bool CameraInRange();
	void ReleaseGridCooldown() { bIsGridOnCooldown = false; }
	

	FTimerHandle TimerHandle_GridCooldown;
	class ABuildableBase* SelectedStructure;
	class AMap_ConstructionGrid* ConstructionGrid; // Actor that is used to handles ConstructionBlockLogic
	bool bIsGridOnCooldown;  //Removes possibility that the player could cause problems due to placing too early.
	bool bInputPressed; // Input is bring pressed
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
	FVector InitialMouseLocation; //The initial location where the mouse was pressed.
	FVector MoveToMousePosition; //The final location the camera should move to.
};


