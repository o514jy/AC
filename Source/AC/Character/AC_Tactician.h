// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AC_CharacterBase.h"
#include "AC_Tactician.generated.h"

class AAC_PlaceableObject1x1;
class AAC_ObjectBase;
class AAC_Champion;

USTRUCT(BlueprintType)
struct FTacticianStat
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int Level = 1;

	UPROPERTY()
	int MaxTeamNum = 1;

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

USTRUCT(BlueprintType)
struct FPlaceableArenaRowArr
{
	GENERATED_BODY()

public:
	TArray<TObjectPtr<AAC_PlaceableObject1x1>> RowPlaceableArenaArr;

	FPlaceableArenaRowArr()
	{
		RowPlaceableArenaArr.Init(nullptr, 8);
	}
};

USTRUCT(BlueprintType)
struct FArenaChampionRowArr
{
	GENERATED_BODY()

public:
	TArray<FString> ArenaChampionRowArr;

	FArenaChampionRowArr()
	{
		ArenaChampionRowArr.Init(FString(), 8);
	}
};

UCLASS()
class AC_API AAC_Tactician : public AAC_CharacterBase
{
	GENERATED_BODY()
	
public:
	AAC_Tactician();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

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

	void SetArenaChampionArr(const FString& key, int i_index, int j_index);
	TArray<FArenaChampionRowArr> GetArenaChampionArr();

	TArray<TObjectPtr<AAC_PlaceableObject1x1>> GetPlaceableWaitingSeat();
	TArray<FPlaceableArenaRowArr> GetPlaceableArena();

	// 이자 불 이펙트 활성화
	void ActivateInterestFire();

private:
	FTacticianStat TacticianStat;

	TArray<FString/*key*/> WaitingChampionArr;
	TArray<FArenaChampionRowArr> ArenaChampionArr;

	TArray<TObjectPtr<AAC_PlaceableObject1x1>> PlaceableWaitingSeat;
	TArray<FPlaceableArenaRowArr> PlaceableArena;

	TArray<TSharedPtr<AAC_Champion>> MyTeamArr;
};
