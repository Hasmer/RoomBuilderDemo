// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MainPawn.generated.h"
class AWallCompound;

UCLASS()
class DEMO_API AMainPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Camera location
	UPROPERTY(EditAnywhere)
	FVector CameraLocation = FVector(0.0f, 0.0f, 200.0f);
	UPROPERTY(EditAnywhere)
	FRotator CameraRotation = FRotator(270.0f, 0.0f, 0.0f);

	// Contains last mouse positions
	FVector2D LastMousePosition;
	FVector LastRayCast;

	// Pointer to Player Controller
	APlayerController* PlayerController;

	// Update mouse position. Return true if mouse position was updated (i.e. new position is not the same as previous)
	bool UpdateMousePosition(FVector2D);

	// Return mouse position
	FVector2D GetMousePosition();

	UPROPERTY(EditAnywhere)
	USceneComponent* Snapper;
	UCameraComponent* ViewerCamera;

	// INPUT LOGIC
	bool bPanning;
	bool bClicking;

	void StartPan();
	void StopPan();

	void CameraZoomIn();
	void CameraZoomOut();

	void OnClickStart();
	void OnClickEnd();

	void Action1();


	FHitResult GetHitResult();
	FHitResult LastHitResult;


	// Utilities
	FVector SnapVector(FVector Vector, float Snap);
	FVector SnapVector(FVector Vector, FVector Snap);

	TSubclassOf<AWallCompound> WallCompoundBP;
	AWallCompound* SelectedWallCompound;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UUserWidget> WidgetTemplate;

	//UPROPERTY(EditAnywhere)
	//UUserWidget* WidgetInstance;

	// The class that will be used for the players Inventory UI
	UPROPERTY(EditAnywhere, Category = "Player, HUD and UI")
		TSubclassOf<class UMainUserWidget> InventoryUIClass;

	// The instance of the players Inventory UI Widget
	UPROPERTY(EditAnywhere, Category = "Player, HUD and UI")
	class UMainUserWidget* InventoryWidget;

	UFUNCTION(BlueprintCallable, Category = "SunShine")
	void SetMode(FString Mode);

	UFUNCTION(BlueprintCallable, Category = "SunShine")
	void EnableSnapping(bool bSnapping);

private:
	TArray<AWallCompound*> WallCompounds;
	FString SelectedComponent;

	FString Mode;
	bool bSnapping;

	AWallCompound* GetWallCompound(AActor* Actor);
};
