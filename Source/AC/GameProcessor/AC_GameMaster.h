// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enum/AC_Enum.h"
#include "AC_GameMaster.generated.h"

struct FChampionInfo;
class AAC_Tactician;
class AAC_Champion;
class IAC_TargetInterface;

USTRUCT(BlueprintType)
struct FChampionKeyArr
{
	GENERATED_BODY()

public:
	TArray<FString> ChampionKeyArr;
};

UCLASS()
class AC_API AAC_GameMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_GameMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FChampionInfo> GetChampionInfoArr();

	void InitChampionPoolAllCost();
	FString PickChampionCardRandomlyUsingKey();
	FString FindSpecificKey(FRandomStream RS, TMap<FString, FChampionKeyArr> championPool);

	AAC_Tactician* GetTactician();

	UFUNCTION()
	void ResellChampionCardUsingKey(const FString& key);
	
	bool SellChampionCardUsingKey(FString key, int championCost);
	bool FindAndPlaceEmptySeat(FString key, TMap<FString, FChampionKeyArr>& championPool);
	
public:
	// Round 설정

	UFUNCTION()
	void SetGame();

	void SetRemainTimeAndRoundState();

	void UpdateRoundState();
	
	void SetPrepareState();
	void SetReadyState();
	void SetBattleState();

	void SetGameRound(int gameRound);

	EGameState GetRoundState() { return RoundState; }

public:
	// 크립 라운드용 팀 설정 (1:창고블린2) (2:창고블린2,새총고블린1) (3:창고블린2,새총고블린2)
	void SpawnCreepTeam(int teamNum);

	TArray<FString> CreepTeamKeyArr;


private:
	UPROPERTY()
	TArray<FChampionInfo> ChampionInfoArr;

	UPROPERTY()
	TObjectPtr<AAC_Tactician> Tactician = nullptr;

	UPROPERTY()
	TMap<FString/*key*/, FChampionKeyArr> ChampionPool1Cost; // 챔피언당 30

	UPROPERTY()
	TMap<FString/*key*/, FChampionKeyArr> ChampionPool2Cost; // 챔피언당 15

	UPROPERTY()
	TMap<FString/*key*/, FChampionKeyArr> ChampionPool3Cost; // 챔피언당 10

private:
	// Timer
	FTimerHandle RoundTimer;

public:
	int GetGameRound() { return GameRound; }

private:
	// Round
	int GameRound = 1;
	bool bRoundStart = false;
	EGameState RoundState = EGameState::Prepare;
	int RemainTime = -1;

};
