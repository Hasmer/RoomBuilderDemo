// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WallCompound.generated.h"
class AWall;

UCLASS()
class DEMO_API AWallCompound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallCompound();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	TSubclassOf<AWall> WallBP;
	AWall* SelectedWall;

	void CreateWall(FVector Location, float Height, float Thickness);
	void SetWallEndPosition(FVector Location);
	void SetWallStartPosition(FVector Location);
	FVector SetWallEndProjection(FVector Location);
	bool IsCompoundClosed();

	FString GetMode();
	void SetMode(FString Mode);

	void DelegateAction(FHitResult HitResult, FVector Location);
	void MakeAction(FVector Location);

	void GetParamsForPerspectiveCamera(FVector &Location, FRotator &Rotator);

private:
	TArray<AWall*> Walls;
	FString Mode;
	int32 WallIndex;

	AWall* GetWall(FHitResult HitResult);
	
};
