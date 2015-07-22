// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo.h"
#include "Wall.h"
#include "WallSection.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	WallEndingStart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallEndingStart"));
	WallEndingEnd = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallEndingEnd"));
	WallEndingStart->AttachTo(RootComponent);
	WallEndingEnd->AttachTo(RootComponent);

	static ConstructorHelpers::FObjectFinder<UBlueprint> WallSection(TEXT("Blueprint'/Game/WallSectionBP.WallSectionBP'"));
	WallSectionBP = (UClass*)WallSection.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
	FVector Loc = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rot = FRotator();
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.Owner = this;
	SelectedWallSection = (AWallSection*)GetWorld()->SpawnActor(WallSectionBP, &Loc, &Rot, SpawnParams);
	SelectedWallSection->SetHeight(59.0f);
	SelectedWallSection->SetSize(10.0f);
	SelectedWallSection->SetThickness(10.0f);
	SelectedWallSection->AttachRootComponentToActor(this);


	//static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("Material'/Game/Wall1.Wall1'"));
	//if (MatFinder.Succeeded())
	//{
	//	Material = MatFinder.Object;
	//	MatDyn = UMaterialInstanceDynamic::Create(Material, this);
	//	MatDyn->SetScalarParameterValue("TextureUParam", (float)FMath::RandRange(2, 5));
	//	MatDyn->SetScalarParameterValue("TextureVParam", (float)FMath::RandRange(2, 5));
	//	WallComponent->SetMaterial(0, MatDyn);
	//}
}

// Called every frame
void AWall::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AWall::SetWallStart(FVector Location)
{
	WallStart = Location;
	SetWallSize(WallStart, WallEnd);
}

void AWall::SetWallEnd(FVector Location)
{
	WallEnd = Location;
	SetWallSize(WallStart, WallEnd);
}

FString AWall::GetOnClickMode()
{
	return TEXT("MOVE_ENDING");
}

FVector AWall::GetWallStartLocation()
{
	return WallEndingStart->GetComponentLocation();
}

FVector AWall::GetWallEndLocation()
{
	return WallEndingEnd->GetComponentLocation();
}

float AWall::SetWallSize(FVector StartLocation, FVector EndLocation)
{
	WallSize = FVector::Dist(StartLocation, EndLocation);
	if (WallSize == 0.0f)
	{
		//WallSize = 10.0f;
	}

	SelectedWallSection->SetSize(WallSize);

	FVector MiddlePoint = FVector();
	MiddlePoint.X = (StartLocation.X + EndLocation.X) / 2.0f;
	MiddlePoint.Y = (StartLocation.Y + EndLocation.Y) / 2.0f;
	MiddlePoint.Z = 0.0f;

	FVector Direction = StartLocation - EndLocation;
	FRotator Rotator = FRotationMatrix::MakeFromZ(Direction).Rotator();
	FString log = "Rotator: ";
	log += Rotator.ToCompactString();
	log += "; Start: ";
	log += WallStart.ToCompactString();
	log += "; End: ";
	log += WallEnd.ToCompactString();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, log);

	this->SetActorLocation(MiddlePoint);
	FRotator CurrentRotator = this->GetActorRotation();
	CurrentRotator.Yaw = Rotator.Yaw;
	this->SetActorRotation(CurrentRotator);


	WallEndingStart->SetRelativeLocation(FVector(0.0f, -WallSize / 2.0f, 0.0f));
	WallEndingEnd->SetRelativeLocation(FVector(0.0f, WallSize / 2.0f, 0.0f));


	return WallSize;
}
