// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AC_DataManager.generated.h"

class UAC_ChampionInfo;
class UAC_StoreProbabilityInfo;
class UAC_TacticianXpInfo;

UCLASS()
class AC_API AAC_DataManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_DataManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UAC_ChampionInfo* GetChampionInfo();
	UAC_StoreProbabilityInfo* GetStoreProbabilityInfo();
	UAC_TacticianXpInfo* GetTacticianXpInfo();
	
private:
	UPROPERTY()
	TObjectPtr<UAC_ChampionInfo> ChampionInfo = nullptr;

	UPROPERTY()
	TObjectPtr<UAC_StoreProbabilityInfo> StoreProbabilityInfo = nullptr;

	UPROPERTY()
	TObjectPtr<UAC_TacticianXpInfo> TacticianXpInfo = nullptr;

};
