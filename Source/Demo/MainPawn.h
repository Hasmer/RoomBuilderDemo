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

	UFUNCTION(BlueprintCallable, Category = "MainPawn")
	bool SetMode(FString Mode);

	UFUNCTION(BlueprintCallable, Category = "MainPawn")
	void EnableSnapping(bool bSnapping);

	UFUNCTION(BlueprintCallable, Category = "MainPawn")
	bool SetCameraMode(bool bPerspective);

private:
	float Height = 300.0f;
	float Thickness = 10.0f;

	TArray<AWallCompound*> WallCompounds;
	FString SelectedComponent;

	FString Mode;
	bool bSnapping;
	bool bCameraPerspective = false;

	AWallCompound* GetWallCompound(AActor* Actor);
};
