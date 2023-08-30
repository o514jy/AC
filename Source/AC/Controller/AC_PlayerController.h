// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "AC_PlayerController.generated.h"


class UNiagaraSystem;
class USplineComponent;
class IAC_TargetInterface;
/**
 * 
 */
UCLASS()
class AC_API AAC_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAC_PlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SetRightMouseButtonAction;

	/** Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SetLeftMouseButtonAction;

	// 최소 이동거리 판정
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	// 자연스러운 경로 찾기
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	bool bAutoRunning = false; // 목적지를 향해 가고있는 상태인지
	void AutoRun();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	//uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();
	virtual void PlayerTick(float DeltaTime) override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();

	void OnSetRightMouseButtonTriggered();
	void OnSetRightMouseButtonReleased();
	
	void OnSetLeftMouseButtonTriggered();
	void OnSetLeftMouseButtonReleased();

private:
	void TickCursorTrace();
	IAC_TargetInterface* TargetActor;

	void TickPickingObject();
	void PickingObject();
	IAC_TargetInterface* PickedActor;

private:
	FVector CachedDestination;

	bool bIsPicked = false;
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

};
