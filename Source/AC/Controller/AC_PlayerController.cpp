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
#include "Engine/GameViewportClient.h"
#include "Kismet/GameplayStatics.h"
// Manager
#include "AC/Managers/AC_ObjectManager.h"
#include "AC/Managers/AC_UIManager.h"
#include "AC/Library/AC_FunctionLibrary.h"
// UI
#include "AC/UI/ChampionStore/AC_ChampionStoreUI.h"
// Object
#include "AC/Object/AC_PlaceableObject1x1.h"


AAC_PlayerController::AAC_PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	ShortPressThreshold = 0.3f;

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

	// 550 1520 상점 ui 직접잰거
	// 921 1080

	if (absoluteScreenX >= 550 && absoluteScreenX <= 1520 && absoluteScreenY >= 920 && absoluteScreenY <= 1080)
	{
		GetTactician()->SubMyTeamArr(Cast<AAC_Champion>(PickedActor));
		ResellToStore.ExecuteIfBound(Cast<AAC_Champion>(PickedActor)->GetObjectKey());
		PickedActor = nullptr;
	}

	Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetSellingButtonAndPrice(false, 0);

	return true;
}

void AAC_PlayerController::SetPlaceableWaitingSeatOnOff(bool flag)
{
	for (int i = 0; i < 8; i++)
	{
		const FString key = Cast<AAC_Tactician>(GetCharacter())->GetObjectKey() + TEXT("PlaceableObject1x1WaitingSeat") + FString::Printf(TEXT("%d"), i + 1);
		UAC_FunctionLibrary::GetObjectManager(GetWorld())->SetObjectOnOff(key, flag);
	}
}

void AAC_PlayerController::SetPlaceableArenaOnOff(bool flag)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			const FString key = Cast<AAC_Tactician>(GetCharacter())->GetObjectKey() + TEXT("PlaceableObject1x1Arena") + FString::Printf(TEXT("%d"), i + 1) + FString::Printf(TEXT("%d"), j + 1);
			UAC_FunctionLibrary::GetObjectManager(GetWorld())->SetObjectOnOff(key, flag);
		}
	}
}

bool AAC_PlayerController::CheckAndPlacePickedActor()
{
	if (PickedActor == nullptr)
		return false;

	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel3, false, CursorHit);

	FVector pickedActorCurrLocation = CursorHit.Location;

	TArray<TObjectPtr<AAC_PlaceableObject1x1>> waitSeatArr = Cast<AAC_Tactician>(GetCharacter())->GetPlaceableWaitingSeat();
	TArray<FPlaceableArenaRowArr> arenaArr = Cast<AAC_Tactician>(GetCharacter())->GetPlaceableArena();

	int overlappedCount = 0;
	float minDistance = 100000;
	FVector currLoc;
	FString currLocKey = FString();

	TPair<int/*i*/, int/*j*/> arenaIndex;
	arenaIndex.Key = -1;
	arenaIndex.Value = -1;
	int waitIndex = -1;

	TPair<int, int> arenaCurrIndex;
	arenaCurrIndex.Key = -1;
	arenaCurrIndex.Value = -1;
	int waitCurrIndex = -1;

	int whatIndex = -1; // wait : 1, arena : 2
	int whatPickedIndex = -1; 

	// 충돌이 일어난 발판과 캐릭터 사이에 거리재서 가장 가까운 곳 기록
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Cast<AAC_Tactician>(GetCharacter())->GetArenaChampionArr()[i].ArenaChampionRowArr[j].Equals(Cast<AAC_Champion>(PickedActor)->GetObjectKey()))
			{
				arenaIndex.Key = i;
				arenaIndex.Value = j;
				whatIndex = 2;
			}

			if (arenaArr[i].RowPlaceableArenaArr[j]->GetbIsOverlapped() == true)
			{
				overlappedCount++;
				float checkDis = FVector::Dist(pickedActorCurrLocation, arenaArr[i].RowPlaceableArenaArr[j]->GetActorLocation());
				
				if (checkDis < minDistance)
				{
					minDistance = checkDis;
					currLocKey = Cast<AAC_Tactician>(GetCharacter())->GetArenaChampionArr()[i].ArenaChampionRowArr[j];
					whatPickedIndex = 2;
					arenaCurrIndex.Key = i;
					arenaCurrIndex.Value = j;
					currLoc = arenaArr[i].RowPlaceableArenaArr[j]->GetActorLocation();
				}
			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (Cast<AAC_Tactician>(GetCharacter())->GetWaitingChampionArr()[i].Equals(Cast<AAC_Champion>(PickedActor)->GetObjectKey()))
		{
			waitIndex = i;
			whatIndex = 1;
		}

		if (waitSeatArr[i]->GetbIsOverlapped() == true)
		{
			overlappedCount++;
			float checkDis = FVector::Dist(pickedActorCurrLocation, waitSeatArr[i]->GetActorLocation());
		
			if (checkDis < minDistance)
			{
				minDistance = checkDis;
				currLocKey = Cast<AAC_Tactician>(GetCharacter())->GetWaitingChampionArr()[i];
				whatPickedIndex = 1;
				waitCurrIndex = i;
				currLoc = waitSeatArr[i]->GetActorLocation();
			}
		}
	}

	if (overlappedCount == 0)
		return false;

	// 교체작업
	FVector tempLoc = pickedActorPrevLocation;
	// 내려놓을 자리에 이미 다른 챔피언이 있었으면 이전 자리와 교체
	if (currLocKey != FString())
	{
		UAC_FunctionLibrary::GetObjectManager(GetWorld())->FindChampion(currLocKey)->SetActorLocation(pickedActorPrevLocation);
	}

	switch (whatIndex) // wait : 1, arena : 2
	{
	case 1:
		Cast<AAC_Tactician>(GetCharacter())->SetWaitingChampionArr(currLocKey, waitIndex);
		break;
	case 2:
		Cast<AAC_Tactician>(GetCharacter())->SetArenaChampionArr(currLocKey, arenaIndex.Key, arenaIndex.Value);
		break;
	default:
		break;
	}

	currLoc.Z += 53.f;
	Cast<AAC_Champion>(PickedActor)->SetActorLocation(currLoc);

	switch (whatPickedIndex) // wait : 1, arena : 2
	{
	case 1:
		Cast<AAC_Tactician>(GetCharacter())->SetWaitingChampionArr(Cast<AAC_Champion>(PickedActor)->GetObjectKey(), waitCurrIndex);
		break;
	case 2:
		Cast<AAC_Tactician>(GetCharacter())->SetArenaChampionArr(Cast<AAC_Champion>(PickedActor)->GetObjectKey(), arenaCurrIndex.Key, arenaCurrIndex.Value);
		break;
	default:
		break;
	}

	return true;
}

AAC_Tactician* AAC_PlayerController::GetTactician()
{
	if (Tactician == nullptr)
	{
		Tactician = Cast<AAC_Tactician>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		Tactician->SetObjectKey(TEXT("Tactician") + FString::Printf(TEXT("%d"), 0));
	}
	return Tactician;
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

		pickedActorPrevLocation = CursorHit.GetActor()->GetActorLocation();
		PickedActor = Cast<IAC_TargetInterface>(CursorHit.GetActor());
		TickPickingObject();
	}
	else
	{
		TickPickingObject();
	}

	if (PickedActor != nullptr)
	{
		SetPlaceableWaitingSeatOnOff(true);
		SetPlaceableArenaOnOff(true);
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

			if (PickedActor != nullptr && CheckAndPlacePickedActor() == false)
				Cast<AAC_Champion>(PickedActor)->SetActorLocation(pickedActorPrevLocation);

			bIsPicked = false;
			PickedActor = nullptr;
		}
	}
	else
	{
		bCheckMousePositionOnStore();

		if (PickedActor != nullptr && CheckAndPlacePickedActor() == false)
			Cast<AAC_Champion>(PickedActor)->SetActorLocation(pickedActorPrevLocation);

		PickedActor = nullptr;
	}

	if (PickedActor == nullptr)
	{
		SetPlaceableWaitingSeatOnOff(false);
		SetPlaceableArenaOnOff(false);
	}

	FollowTime = 0.f;
}

void AAC_PlayerController::TickCursorTrace()
{
	FHitResult CursorHit;

	IAC_TargetInterface* LocalTargetActor;

	if (PickedActor == nullptr)
	{
		if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit) == false)
			return;

		LocalTargetActor = Cast<IAC_TargetInterface>(CursorHit.GetActor());
	}
	else
	{
		LocalTargetActor = PickedActor;
	}

	if (LocalTargetActor == nullptr)
	{
		// 원래 있었는데, 없어짐
		if (TargetActor)
		{
			TargetActor->UnHighlightActor();
		}
	}
	else
	{
		// 적팀일 경우 표시X
		if (Cast<AAC_Champion>(LocalTargetActor)->GetbIsEnemy() == true)
			return;

		if (TargetActor)
		{
			// 원래 있었는데 다른 애였음
			if (TargetActor != LocalTargetActor)
			{
				TargetActor->UnHighlightActor();
				LocalTargetActor->HighlightActor();
			}
			// 동일한 애면 무시
		}
		else
		{
			// 원래 없었고 새로 주시
			LocalTargetActor->HighlightActor();
		}
	}

	// 저장
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

	if (pickedChampion->GetbIsEnemy() == true)
	{
		PickedActor = nullptr;
		return;
	}

	if (championStoreUI->GetSellingButtonVisible() == false)
	{
		championStoreUI->SetSellingButtonAndPrice(true, pickedChampion->GetChampionStat().ChampionCost);
	}

	FHitResult CursorHit;

	// GameTraceChannel3 = TraceFloor
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel3, false, CursorHit);
	//GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHitTemp);
	
	// 커서에 캐릭터 붙이기
	APawn* PickedPawn = Cast<APawn>(PickedActor);
	FVector LocationXYZ = CursorHit.Location;
	LocationXYZ.Z = PickedPawn->GetActorLocation().Z;
	

	FVector ActorLocation = pickedChampion->GetActorLocation();
	FVector Start = ActorLocation;
	FVector End = ActorLocation;
	End.Z -= 1000.0f; // 바닥을 체크할 레이캐스트의 길이를 지정합니다.

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(pickedChampion); // 레이캐스트에서 현재 액터를 무시합니다.

	// 레이캐스트를 수행하여 바닥과의 충돌을 검사합니다.
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel3, CollisionParams))
	{
		// 바닥과 충돌한 경우, 액터의 위치를 조정합니다.
		FVector NewActorLocation = HitResult.ImpactPoint;
		LocationXYZ.Z = NewActorLocation.Z + 53.f;
	}


	PickedPawn->SetActorLocation(LocationXYZ);
}


