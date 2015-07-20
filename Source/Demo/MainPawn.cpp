// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo.h"
#include "MainPawn.h"
#include "Wall.h"

// Sets default values
AMainPawn::AMainPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ViewerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewerCamera"));
	ViewerCamera->AttachTo(RootComponent);
	ViewerCamera->SetRelativeLocation(CameraLocation);
	ViewerCamera->SetRelativeRotation(CameraRotation);

	Snapper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Snapper"));
	Snapper->AttachTo(RootComponent);


	static ConstructorHelpers::FObjectFinder<UBlueprint> Wall(TEXT("Blueprint'/Game/WallBP.WallBP'"));
	WallBP = (UClass*)Wall.Object->GeneratedClass;
	Walls = TArray<AWall*>();
}

// Called when the game starts or when spawned
void AMainPawn::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = GetWorld()->GetFirstPlayerController();
}

// Called every frame
void AMainPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector2D NewMousePosition = GetMousePosition();

	if (UpdateMousePosition(NewMousePosition))
	{
		FHitResult HitResult = GetHitResult();

		if (bPanning)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Panning"));
			FVector Delta = HitResult.Location - LastHitResult.Location;
			Delta.Z = 0.0f;
			FString log = Delta.ToCompactString();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, log);
			ViewerCamera->AddRelativeLocation(-Delta);
			LastHitResult = GetHitResult();
			LastHitResult.Location -= Delta;
		}
		else if (bClicking)
		{
			if (SelectedWall)
			{
				FVector CursorLocation = SnapVector(HitResult.Location, 10.0f);
				CursorLocation.Z = 0.0f;
				if (SelectedComponent == "WallEndingEnd")
				{
					SelectedWall->SetWallEnd(CursorLocation);
					int SelectedIndex = Walls.Find(SelectedWall);
					if (SelectedIndex != INDEX_NONE)
					{
						if (SelectedIndex < Walls.Num() - 1)
						{
							AWall* ConnectedWall = Walls[SelectedIndex + 1];
							ConnectedWall->SetWallStart(CursorLocation);
						}
					}
				}
				else if (SelectedComponent == "WallEndingStart")
				{
					SelectedWall->SetWallStart(CursorLocation);
					int SelectedIndex = Walls.Find(SelectedWall);
					if (SelectedIndex != INDEX_NONE)
					{
						if (SelectedIndex > 0)
						{
							AWall* ConnectedWall = Walls[SelectedIndex - 1];
							ConnectedWall->SetWallEnd(CursorLocation);
						}
					}
				}
			}
		}
		else
		{
			FString log = HitResult.Location.ToCompactString();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, log);

			FVector SnapperPosition = SnapVector(HitResult.Location, 10.0f);
			SnapperPosition.Z = 20.0f;
			Snapper->SetRelativeLocation(SnapperPosition);
		}
	}



}

// Called to bind functionality to input
void AMainPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Pan", IE_Pressed, this, &AMainPawn::StartPan);
	InputComponent->BindAction("Pan", IE_Released, this, &AMainPawn::StopPan);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AMainPawn::CameraZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AMainPawn::CameraZoomOut);
	InputComponent->BindAction("Click", IE_Pressed, this, &AMainPawn::OnClickStart);
	InputComponent->BindAction("Click", IE_Released, this, &AMainPawn::OnClickEnd);

	InputComponent->BindAction("Action1", IE_Pressed, this, &AMainPawn::Action1);
}

// Update mouse position. Return true if mouse position was updated (i.e. new position is not the same as previous)
bool AMainPawn::UpdateMousePosition(FVector2D NewMousePosition)
{
	if (!LastMousePosition.Equals(NewMousePosition, 0.001))
	{
		LastMousePosition = NewMousePosition;
		return true;
	}
	return false;
}

// Return new mouse position
FVector2D AMainPawn::GetMousePosition()
{
	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	return MousePosition;
}

// Trace mouse location and return HitResult
FHitResult AMainPawn::GetHitResult()
{
	FHitResult HitResult = FHitResult();
	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, HitResult);
	AActor* TheActor = HitResult.GetActor();
	UPrimitiveComponent* TheComponent = HitResult.GetComponent();

	if (TheActor)
	{
		FString log = TheActor->GetClass()->GetName();
		if (TheComponent)
		{
			log += " ";
			log += TheComponent->GetName();
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, log);
	}
	return HitResult;
}






///////////////////////////////////////////////////////////
//INPUT CALLBACKS
///////////////////////////////////////////////////////////

// Begin panning
void AMainPawn::StartPan()
{
	bPanning = true;
	LastHitResult = GetHitResult();
}

// End panning
void AMainPawn::StopPan()
{
	bPanning = false;
}

// Camera Zoom In
void AMainPawn::CameraZoomIn()
{
	ViewerCamera->AddRelativeLocation(FVector(0.0f, 0.0f, -5.0f));
}

// Camera Zoom Out
void AMainPawn::CameraZoomOut()
{
	ViewerCamera->AddRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
}

// Begin Click
void AMainPawn::OnClickStart()
{
	bClicking = true;

	FVector2D NewMousePosition = GetMousePosition();
	FHitResult HitResult = GetHitResult();

	AActor* TheActor = HitResult.GetActor();

	if (TheActor)
	{
		FString log = TheActor->GetClass()->GetName();
		if (log == TEXT("WallBP_C"))
		{
			SelectedWall = (AWall*)TheActor;
			UPrimitiveComponent* TheComponent = HitResult.GetComponent();
			if (TheComponent)
			{
				SelectedComponent = TheComponent->GetName();
			}
		}
		else
		{
			FVector Loc = HitResult.Location;
			Loc = SnapVector(Loc, 10.0f);
			Loc.Z = 0.0f;
			AWall* MyWall = (AWall*)GetWorld()->SpawnActor(WallBP, &Loc);
			MyWall->SetWallStart(Loc);
			MyWall->SetWallEnd(Loc);
			Walls.Push(MyWall);
			SelectedWall = MyWall;
			SelectedComponent = TEXT("WallEndingEnd");
			//MyWall->WallComponent->SetMaterial()
		}
	}




}

// End Click
void AMainPawn::OnClickEnd()
{
	bClicking = false;
	SelectedWall = NULL;
}

// Action1
void AMainPawn::Action1()
{
	FVector2D NewMousePosition = GetMousePosition();
	FHitResult HitResult = GetHitResult();

	AActor* TheActor = HitResult.GetActor();

	if (TheActor)
	{
		FString log = TEXT("Action1: ");
		log += TheActor->GetClass()->GetName();
		if (log == TEXT("WallBP_C"))
		{
			SelectedWall = (AWall*)TheActor;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, log);



			SelectedWall = NULL;
		}
	}

}


///////////////////////////////////////////////////////////
// UTILITIES
///////////////////////////////////////////////////////////

// Snap vector to grid
FVector AMainPawn::SnapVector(FVector Vector, float Snap = 10.0f)
{
	FVector SnappedVector = FVector();
	SnappedVector.X = FMath::GridSnap(Vector.X, Snap);
	SnappedVector.Y = FMath::GridSnap(Vector.Y, Snap);
	SnappedVector.Z = FMath::GridSnap(Vector.Z, Snap);
	return SnappedVector;
}

// Snap Vector to grid
FVector AMainPawn::SnapVector(FVector Vector, FVector Snap = FVector(10.0f, 10.0f, 10.0f))
{
	FVector SnappedVector = FVector();
	SnappedVector.X = FMath::GridSnap(Vector.X, Snap.X);
	SnappedVector.Y = FMath::GridSnap(Vector.Y, Snap.Y);
	SnappedVector.Z = FMath::GridSnap(Vector.Z, Snap.Z);
	return SnappedVector;
}
