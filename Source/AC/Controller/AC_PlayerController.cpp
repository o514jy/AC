// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AC_PlayerController.h"
#include "GameFramework/Pawn.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "Math/UnrealMathUtility.h"
#include "../Interface/AC_TargetInterface.h"
#include "AC/Character/AC_Champion.h"
#include "AC/Character/AC_Tactician.h"
// Manager
#include "AC/Managers/AC_ObjectManager.h"
#include "AC/Managers/AC_UIManager.h"
#include "AC/Library/AC_FunctionLibrary.h"
// UI
#include "AC/UI/ChampionStore/AC_ChampionStoreUI.h"

#include "Engine/GameViewportClient.h"

AAC_PlayerController::AAC_PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	ShortPressThreshold = 0.2f;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAC_PlayerController::AutoRun()
{
	if (bAutoRunning == false)
		return;

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

bool AAC_PlayerController::bCheckMousePositionOnStore()
{
	if (PickedActor == nullptr)
		return false;

	FHitResult Hit;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	FVector2D ScreenLocation;
	ProjectWorldLocationToScreen(Hit.Location, ScreenLocation);

	int32 ScreenWidth = 0;
	int32 ScreenHeight = 0;
	GetViewportSize(ScreenWidth, ScreenHeight);

	int32 absoluteScreenX = (int32)ScreenLocation.X * 1980 / ScreenWidth;
	int32 absoluteScreenY = (int32)ScreenLocation.Y * 1080 / ScreenHeight;

	// 550 1520 ���� ui �������
	// 921 1080

	if (absoluteScreenX >= 550 && absoluteScreenX <= 1520 && absoluteScreenY >= 920 && absoluteScreenY <= 1080)
	{
		ResellToStore.ExecuteIfBound(Cast<AAC_Champion>(PickedActor)->GetObjectKey());
	}

	Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetSellingButtonAndPrice(false, 0);

	return true;
}

void AAC_PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup right mouse input events
		EnhancedInputComponent->BindAction(SetRightMouseButtonAction, ETriggerEvent::Started, this, &AAC_PlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetRightMouseButtonAction, ETriggerEvent::Triggered, this, &AAC_PlayerController::OnSetRightMouseButtonTriggered);
		EnhancedInputComponent->BindAction(SetRightMouseButtonAction, ETriggerEvent::Completed, this, &AAC_PlayerController::OnSetRightMouseButtonReleased);
		EnhancedInputComponent->BindAction(SetRightMouseButtonAction, ETriggerEvent::Canceled, this, &AAC_PlayerController::OnSetRightMouseButtonReleased);
	
		// Setup left mouse input events
		EnhancedInputComponent->BindAction(SetLeftMouseButtonAction, ETriggerEvent::Started, this, &AAC_PlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetLeftMouseButtonAction, ETriggerEvent::Triggered, this, &AAC_PlayerController::OnSetLeftMouseButtonTriggered);
		EnhancedInputComponent->BindAction(SetLeftMouseButtonAction, ETriggerEvent::Completed, this, &AAC_PlayerController::OnSetLeftMouseButtonReleased);
		EnhancedInputComponent->BindAction(SetLeftMouseButtonAction, ETriggerEvent::Canceled, this, &AAC_PlayerController::OnSetLeftMouseButtonReleased);
	}
}

void AAC_PlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	FInputModeGameAndUI inputmode = FInputModeGameAndUI();
	inputmode.SetHideCursorDuringCapture(false);
	SetInputMode(inputmode);
}

void AAC_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	AutoRun();
	TickCursorTrace();
	TickPickingObject();
}

void AAC_PlayerController::OnInputStarted()
{
	StopMovement();
}

void AAC_PlayerController::OnSetRightMouseButtonTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AAC_PlayerController::OnSetRightMouseButtonReleased()
{
	APawn* ControlledPawn = GetPawn();
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();
			for (const FVector& PointLoc : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				//DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 1.f);
			}
			CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
			bAutoRunning = true;
		}
	}

	FollowTime = 0.f;
}

void AAC_PlayerController::OnSetLeftMouseButtonTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	if (PickedActor == nullptr)
	{
		FHitResult CursorHit;

		if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit) == false)
			return;

		PickedActor = Cast<IAC_TargetInterface>(CursorHit.GetActor());
		TickPickingObject();
	}
	else
	{
		TickPickingObject();
	}

	if (PickedActor != nullptr)
	{
		FString key1 = Cast<AAC_Tactician>(GetCharacter())->GetObjectKey() + TEXT("PlaceableObject1x4ForWaitingSeat1234");
		FString key2 = Cast<AAC_Tactician>(GetCharacter())->GetObjectKey() + TEXT("PlaceableObject1x4ForWaitingSeat5678");
		UAC_FunctionLibrary::GetObjectManager(GetWorld())->SetObjectOnOff(key1, true);
		UAC_FunctionLibrary::GetObjectManager(GetWorld())->SetObjectOnOff(key2, true);
	}
}

void AAC_PlayerController::OnSetLeftMouseButtonReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		if (bIsPicked == false)
		{
			bIsPicked = true;
		}
		else
		{
			bCheckMousePositionOnStore();
			bIsPicked = false;
			PickedActor = nullptr;
		}
	}
	else
	{
		bCheckMousePositionOnStore();
		PickedActor = nullptr;
	}

	if (PickedActor == nullptr)
	{
		FString key1 = Cast<AAC_Tactician>(GetCharacter())->GetObjectKey() + TEXT("PlaceableObject1x4ForWaitingSeat1234");
		FString key2 = Cast<AAC_Tactician>(GetCharacter())->GetObjectKey() + TEXT("PlaceableObject1x4ForWaitingSeat5678");
		UAC_FunctionLibrary::GetObjectManager(GetWorld())->SetObjectOnOff(key1, false);
		UAC_FunctionLibrary::GetObjectManager(GetWorld())->SetObjectOnOff(key2, false);
	}

	FollowTime = 0.f;
}

void AAC_PlayerController::TickCursorTrace()
{
	FHitResult CursorHit;

	if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit) == false)
		return;

	IAC_TargetInterface* LocalTargetActor = Cast<IAC_TargetInterface>(CursorHit.GetActor());

	if (LocalTargetActor == nullptr)
	{
		// ���� �־��µ�, ������
		if (TargetActor)
		{
			TargetActor->UnHighlightActor();
		}
	}
	else
	{
		if (TargetActor)
		{
			// ���� �־��µ� �ٸ� �ֿ���
			if (TargetActor != LocalTargetActor)
			{
				TargetActor->UnHighlightActor();
				LocalTargetActor->HighlightActor();
			}
			// ������ �ָ� ����
		}
		else
		{
			// ���� ������ ���� �ֽ�
			LocalTargetActor->HighlightActor();
		}
	}

	// ����
	TargetActor = LocalTargetActor;
}

void AAC_PlayerController::TickPickingObject()
{
	if (PickedActor != nullptr)
	{
		PickingObject();
	}
}

void AAC_PlayerController::PickingObject()
{
	UAC_ChampionStoreUI* championStoreUI = Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI));
	AAC_Champion* pickedChampion = Cast<AAC_Champion>(PickedActor);

	if (championStoreUI->GetSellingButtonVisible() == false)
	{
		championStoreUI->SetSellingButtonAndPrice(true, pickedChampion->GetChampionStat().ChampionCost);
	}

	FHitResult CursorHit;

	// GameTraceChannel3 = TraceFloor
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel3, false, CursorHit);
	//GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHitTemp);
	
	// Ŀ���� ĳ���� ���̱�
	APawn* PickedPawn = Cast<APawn>(PickedActor);
	FVector LocationXYZ = CursorHit.Location;
	LocationXYZ.Z = PickedPawn->GetActorLocation().Z;
	PickedPawn->SetActorLocation(LocationXYZ);
}


