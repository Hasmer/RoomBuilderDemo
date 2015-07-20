// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WallSection.generated.h"

UCLASS()
class DEMO_API AWallSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallSection();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WallComponent;

	UPROPERTY(EditAnywhere)
	float ModelSizeInMilimeters = 100.0f;

	float GetSize();
	void SetSize(float Size);
	float GetHeight();
	void SetHeight(float Height);
	float GetThickness();
	void SetThickness(float Thickness);

private:
	float Size = 10.0f;
	float Height = 50.0f;
	float Thickness = 10.0f;
	void UpdateSize();
	
	
};