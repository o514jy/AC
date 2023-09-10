// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AC_AIController.generated.h"

/**
 * 
 */
UCLASS()
class AC_API AAC_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAC_AIController();

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

};
