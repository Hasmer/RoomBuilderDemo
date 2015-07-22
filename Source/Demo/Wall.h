// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Wall.generated.h"
class AWallSection;

UCLASS()
class DEMO_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WallEndingStart;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WallEndingEnd;
	UPROPERTY(EditAnywhere)
	float WallThickness;

	TSubclassOf<AWallSection> WallSectionBP;
	AWallSection* SelectedWallSection;



	FVector WallStart;
	FVector WallEnd;

	// Methods for changing wall size
	void SetWallStart(FVector Location);
	void SetWallEnd(FVector Location);


	UPROPERTY(EditAnywhere)
	UMaterial* Material;

	UMaterialInstanceDynamic* MatDyn;

	FString GetOnClickMode();

	FVector GetWallStartLocation();
	FVector GetWallEndLocation();

private:
	float WallSize;
	float SetWallSize(FVector StartLocation, FVector EndLocation);


};
