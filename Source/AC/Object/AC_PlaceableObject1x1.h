// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/AC_ObjectBase.h"
#include "AC_PlaceableObject1x1.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class AC_API AAC_PlaceableObject1x1 : public AAC_ObjectBase
{
	GENERATED_BODY()
	
public:

	AAC_PlaceableObject1x1();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UBoxComponent* Box = nullptr;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool GetbIsOverlapped() { return bIsOverlapped; }

private:
	bool bIsOverlapped = false;
};
