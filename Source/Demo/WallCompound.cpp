// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo.h"
#include "WallCompound.h"
#include "Wall.h"

// Sets default values
AWallCompound::AWallCompound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> Wall(TEXT("Blueprint'/Game/WallBP.WallBP'"));
	WallBP = (UClass*)Wall.Object->GeneratedClass;
	Walls = TArray<AWall*>();
}

// Called when the game starts or when spawned
void AWallCompound::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallCompound::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AWallCompound::CreateWall(FVector Location)
{
	if (SelectedWall)
	{
		SelectedWall->SetWallEnd(Location);
		if (IsCompoundClosed())
		{
			return;
		}
	}
	SelectedWall = (AWall*)GetWorld()->SpawnActor(WallBP, &Location);
	SelectedWall->AttachRootComponentToActor(this);
	SelectedWall->SetWallStart(Location);
	SelectedWall->SetWallEnd(Location);
	Walls.Push(SelectedWall);
}

void AWallCompound::SetWallEndPosition(FVector Location)
{
	if (SelectedWall)
	{
		SelectedWall->SetWallEnd(Location);
	}
}

void AWallCompound::SetWallStartPosition(FVector Location)
{
	if (SelectedWall)
	{
		SelectedWall->SetWallStart(Location);
	}
}

FVector AWallCompound::SetWallEndProjection(FVector Location)
{
	for (int i = 0; i < Walls.Num(); i++)
	{
		FVector WallStartLocation = Walls[i]->GetWallStartLocation();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Wall " + FString::FromInt(i) + ": " + WallStartLocation.ToCompactString());
		if (FVector::Dist(Location, WallStartLocation) <= 10.0f)
		{
			SelectedWall->SetWallEnd(WallStartLocation);
			return WallStartLocation;
		}
	}
	if (SelectedWall)
	{
		SelectedWall->SetWallEnd(Location);
	}
	return Location;
}

bool AWallCompound::IsCompoundClosed()
{
	int32 WallsCount = Walls.Num();
	if (WallsCount < 2)
	{
		return false;
	}
	AWall* FirstWall = Walls[0];
	AWall* LastWall = Walls[WallsCount - 1];
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, "====Start: " + FirstWall->WallStart.ToCompactString() + "; End: " + LastWall->WallEnd.ToCompactString() + " ====" + (FirstWall->WallStart == LastWall->WallEnd ? "EQUAL" : "NON EQUAL"));
	return FirstWall->WallStart.Equals(LastWall->WallEnd, 0.01f);
}

FString AWallCompound::GetMode()
{
	return Mode;
}

void AWallCompound::SetMode(FString Mode)
{
	this->Mode = Mode;
}

// Delegate Action function define action/mode by hit result passed. Location is used in action
void AWallCompound::DelegateAction(FHitResult HitResult, FVector Location)
{
	AActor* Actor = HitResult.GetActor();
	SelectedWall = GetWall(HitResult);
	if (SelectedWall)
	{
		UPrimitiveComponent* TheComponent = HitResult.GetComponent();
		if (TheComponent)
		{
			FString ComponentName = TheComponent->GetName();
			if (ComponentName == "WallEndingStart")
			{
				int32 SelectedWallIndex = Walls.Find(SelectedWall);
				if (SelectedWallIndex == 0)
				{
					SelectedWallIndex = Walls.Num();
				}
				SelectedWall = Walls[SelectedWallIndex - 1];
				SetMode("MOVE_ENDING");
			}
			else if (ComponentName == "WallEndingEnd")
			{
				SetMode("MOVE_ENDING");
			}
		}
	}
}

void AWallCompound::MakeAction(FVector Location)
{
	if (Mode == "MOVE_ENDING")
	{
		SelectedWall->SetWallEnd(Location);
		int32 SelectedWallIndex = Walls.Find(SelectedWall);
		int32 NextWallIndex;
		if (SelectedWallIndex == Walls.Num() - 1)
		{
			NextWallIndex = 0;
		}
		else
		{
			NextWallIndex = SelectedWallIndex + 1;
		}
		Walls[NextWallIndex]->SetWallStart(Location);
	}
}

AWall* AWallCompound::GetWall(FHitResult HitResult)
{
	AActor* Actor = HitResult.GetActor();
	FString ActorClass;
	while (Actor)
	{
		ActorClass = Actor->GetClass()->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "GettingWall..." + ActorClass);
		if (ActorClass == "WallBP_C")
		{
			return (AWall*)Actor;
		}
		Actor = Actor->GetAttachParentActor();
	}
	return NULL;
}