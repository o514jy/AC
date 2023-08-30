// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProcessor/AC_GameMaster.h"
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Managers/AC_DataManager.h"
#include "AC/Data/AC_ChampionInfo.h"
#include "AC/Data/AC_StoreProbabilityInfo.h"
#include "AC/Character/AC_Tactician.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAC_GameMaster::AAC_GameMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAC_GameMaster::BeginPlay()
{
	Super::BeginPlay();
	InitChampionPoolAllCost();
	PickAtRandomChampionCardName();
}

// Called every frame
void AAC_GameMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FChampionInfo> AAC_GameMaster::GetChampionInfoArr()
{
	if (ChampionInfoArr.IsEmpty())
	{
		ChampionInfoArr = UAC_FunctionLibrary::GetDataManager(GetWorld())->GetChampionInfo()->ChampionInfoArr;
	}

	return ChampionInfoArr;
}

void AAC_GameMaster::InitChampionPoolAllCost()
{
	for (FChampionInfo info : GetChampionInfoArr())
	{
		switch (info.ChampionCost)
		{
		case 1:
			ChampionPool1Cost.Add(info.ChampionName, 30);
			break;
		case 2:
			ChampionPool2Cost.Add(info.ChampionName, 15);
			break;
		case 3:
			ChampionPool3Cost.Add(info.ChampionName, 10);
			break;
		}
	}
}

FString AAC_GameMaster::PickAtRandomChampionCardName()
{
	int level = GetTactician()->GetTacticianStat().Level;
	TMap<int/*Tactician Level*/, FProbabilityInfoArr> storeMap = UAC_FunctionLibrary::GetDataManager(GetWorld())->GetStoreProbabilityInfo()->StoreProbabilityInfoMap;

	FRandomStream RS;
	RS.GenerateNewSeed();
	int costProb = RS.RandRange(1, 100);

	TArray<int> probArr = storeMap.Find(level)->ProbabilityInfoArr;

	FString PickedChampionName;

	if (costProb <= probArr[0])
	{
		// 1코스트 챔피언 생성
		PickedChampionName = FindSpecificName(RS, ChampionPool1Cost);
	}
	else if (costProb > probArr[0] && costProb <= probArr[0] + probArr[1])
	{
		// 2코스트 챔피언 생성
		PickedChampionName = FindSpecificName(RS, ChampionPool2Cost);
	}
	else
	{
		// 3코스트 챔피언 생성
		PickedChampionName = FindSpecificName(RS, ChampionPool3Cost);
	}

	return PickedChampionName;
}

FString AAC_GameMaster::FindSpecificName(FRandomStream RS, TMap<FString, int> championPool)
{
	RS.GenerateNewSeed();

	int poolSize = 0;

	TArray<int> elemArr;
	for (auto& elem : championPool)
	{
		poolSize += elem.Value;
		elemArr.Add(elem.Value);
	}

	int championProb = RS.RandRange(1, poolSize);
	int weight = 0;
	for (auto& elem : championPool)
	{
		weight += elem.Value;
		if (weight >= championProb)
		{
			return elem.Key;
		}
	}

	return FString();
}

AAC_Tactician* AAC_GameMaster::GetTactician()
{
	if (Tactician == nullptr)
	{
		Tactician = Cast<AAC_Tactician>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	return Tactician;
}

bool AAC_GameMaster::SellingChampionForName(FString championName, int championCost)
{
	switch (championCost)
	{
	case 1:
		if (ChampionPool1Cost[championName] <= 0)
			return false;
		ChampionPool1Cost[championName]--;
		break;
	case 2:
		if (ChampionPool2Cost[championName] <= 0)
			return false;
		ChampionPool2Cost[championName]--;
		break;
	case 3:
		if (ChampionPool3Cost[championName] <= 0)
			return false;
		ChampionPool3Cost[championName]--;
		break;
	default:
		break;
	}

	return true;
}

