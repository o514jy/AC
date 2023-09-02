// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AC_CharacterBase.h"
#include "AC_Tactician.generated.h"

USTRUCT(BlueprintType)
struct FTacticianStat
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int Level = 1;

	UPROPERTY()
	int Xp = 0;

	UPROPERTY()
	int maxXp = 0;

	UPROPERTY()
	int PossessionGold = 70;

	UPROPERTY()
	int Health = 100;

	UPROPERTY()
	float MoveSpeedRatio = 1;

	UPROPERTY()
	float TacticianSizeRatio = 1;

	
};

UCLASS()
class AC_API AAC_Tactician : public AAC_CharacterBase
{
	GENERATED_BODY()
	
public:
	AAC_Tactician();

	FTacticianStat GetTacticianStat();

	void SetLevel(int level);

	void SetXp(int xp);

	void SetMaxXp(int maxXp);

	void addXp(int addXp);

	void SetPossessionGold(int gold);

	void AddPossessionGold(int addGold);

	bool SubPossessionGold(int subGold);

	void SetWaitingChampionArr(const FString& key, int index);
	TArray<FString> GetWaitingChampionArr();

	void ActivateInterestFire();

private:
	FTacticianStat TacticianStat;

	TArray<FString/*key*/> WaitingChampionArr;
	TArray<FString/*Key*/> ArenaChampionArr;
};
