// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo.h"
#include "WallSection.h"


// Sets default values
AWallSection::AWallSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	WallComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallComponent"));
	WallComponent->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void AWallSection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallSection::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

float AWallSection::GetSize()
{
	return Size;
}

void AWallSection::SetSize(float Size)
{
	this->Size = Size;
	UpdateSize();
}

float AWallSection::GetHeight()
{
	return Height;
}

void AWallSection::SetHeight(float Height)
{
	this->Height = Height;
	UpdateSize();
}

float AWallSection::GetThickness()
{
	return Thickness;
}

void AWallSection::SetThickness(float Thickness)
{
	this->Thickness = Thickness;
	UpdateSize();
}

void AWallSection::UpdateSize()
{
	FVector Scale = WallComponent->GetComponentScale();
	Scale.X = Thickness / ModelSizeInMilimeters;
	Scale.Y = Size / ModelSizeInMilimeters;
	Scale.Z = Height / ModelSizeInMilimeters;
	WallComponent->SetRelativeScale3D(Scale);
}