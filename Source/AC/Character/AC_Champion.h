// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AC_CharacterBase.h"
#include "../Interface/AC_TargetInterface.h"
#include "AC_Champion.generated.h"

USTRUCT(BlueprintType)
struct FChampionStat
{
	GENERATED_BODY()

public:
	// Combat Stat
	UPROPERTY()
	int Health;

	UPROPERTY()
	int AttackRange;

	UPROPERTY()
	int AttackDamage;

	UPROPERTY()
	float AttackSpeedRatio;

	UPROPERTY()
	int AbilityPower;

	UPROPERTY()
	float MoveSpeedRatio;

	// Info Stat
	UPROPERTY()
	int ChampionCost;

	UPROPERTY()
	int StarLevel;

	UPROPERTY()
	FString Ethnic;

	UPROPERTY()
	FString Occupation;
};

UCLASS()
class AC_API AAC_Champion : public AAC_CharacterBase, public IAC_TargetInterface
{
	GENERATED_BODY()
	
public:
	AAC_Champion();

	virtual void BeginPlay() override;

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

};
