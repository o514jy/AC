// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AC_ChampionInfo.generated.h"

USTRUCT(BlueprintType)
struct FChampionInfo
{
	GENERATED_BODY()

public:
	// Value Info
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ChampionName = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Ethnic = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Occupation = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ChampionCost = 0;

	// Stat Info
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ChampionStarLevel = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Health = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int AttackRange = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int AttackDamage = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackSpeedRatio = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int AbilityPower = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MoveSpeedRatio = 0.f;
};

UCLASS()
class AC_API UAC_ChampionInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FChampionInfo FindChampionInfoForChampionName(const FString& championName, bool bLogNotFound = false);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FChampionInfo> ChampionInfoArr;
};
