// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AC_EnvObject.generated.h"

class AStaticMeshActor;

UCLASS()
class AC_API AAC_EnvObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_EnvObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<TObjectPtr<AActor>> GetarrOutActors();

	TObjectPtr<AActor> GetMainCamera();

	TArray<TObjectPtr<AStaticMeshActor>> GetWaitingSeat();

	TArray<TObjectPtr<AStaticMeshActor>> GetArena();

	TObjectPtr<AStaticMeshActor> GetItemSeat();

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> arrOutActors;

	UPROPERTY()
	TObjectPtr<AActor> MainCamera;

	UPROPERTY()
	TArray<TObjectPtr<AStaticMeshActor>> SM_WaitingSeat;

	UPROPERTY()
	TArray<TObjectPtr<AStaticMeshActor>> SM_Arena;

	UPROPERTY()
	TObjectPtr<AStaticMeshActor> SM_ItemSeat;
};
