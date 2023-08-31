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
	PickChampionCardRandomlyUsingKey();
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
		FChampionKeyArr CKQ;
		switch (info.ChampionCost)
		{
		case 1:
			for (int i = 30; i > 0; i--)
			{
				const FString name = info.Key + FString::Printf(TEXT("%d"), i);
				CKQ.ChampionKeyArr.Add(name);
			}
			ChampionPool1Cost.Add(info.Key, CKQ);
			break;
		case 2:
			for (int i = 15; i > 0; i--)
			{
				const FString name = info.Key + FString::Printf(TEXT("%d"), i);
				CKQ.ChampionKeyArr.Add(name);
			}
			ChampionPool2Cost.Add(info.Key, CKQ);
			break;
		case 3:
			for (int i = 10; i > 0; i--)
			{
				const FString name = info.Key + FString::Printf(TEXT("%d"), i);
				CKQ.ChampionKeyArr.Add(name);
			}
			ChampionPool3Cost.Add(info.Key, CKQ);
			break;
		}
	}
}

FString AAC_GameMaster::PickChampionCardRandomlyUsingKey()
{
	int level = GetTactician()->GetTacticianStat().Level;
	TMap<int/*Tactician Level*/, FProbabilityInfoArr> storeMap = UAC_FunctionLibrary::GetDataManager(GetWorld())->GetStoreProbabilityInfo()->StoreProbabilityInfoMap;

	FRandomStream RS;
	RS.GenerateNewSeed();
	int costProb = RS.RandRange(1, 100);

	TArray<int> probArr = storeMap.Find(level)->ProbabilityInfoArr;

	FString PickedKey;

	if (costProb <= probArr[0])
	{
		// 1코스트 챔피언 생성
		PickedKey = FindSpecificKey(RS, ChampionPool1Cost);
	}
	else if (costProb > probArr[0] && costProb <= probArr[0] + probArr[1])
	{
		// 2코스트 챔피언 생성
		PickedKey = FindSpecificKey(RS, ChampionPool2Cost);
	}
	else
	{
		// 3코스트 챔피언 생성
		PickedKey = FindSpecificKey(RS, ChampionPool3Cost);
	}

	return PickedKey;
}

FString AAC_GameMaster::FindSpecificKey(FRandomStream RS, TMap<FString, FChampionKeyArr> championPool)
{
	RS.GenerateNewSeed();

	int poolSize = 0;

	TArray<int> elemArr;
	for (auto& elem : championPool)
	{
		poolSize += elem.Value.ChampionKeyArr.Num();
		elemArr.Add(elem.Value.ChampionKeyArr.Num());
	}

	int championProb = RS.RandRange(1, poolSize);
	int weight = 0;
	for (auto& elem : championPool)
	{
		weight += elem.Value.ChampionKeyArr.Num();
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

bool AAC_GameMaster::SellChampionCardUsingKey(FString key, int championCost)
{
	int arrSize;
	switch (championCost)
	{
	case 1:
		arrSize = ChampionPool1Cost[key].ChampionKeyArr.Num();
		if (arrSize <= 0)
			return false;
		ChampionPool1Cost[key].ChampionKeyArr.RemoveAt(arrSize - 1);
		break;
	case 2:
		arrSize = ChampionPool2Cost[key].ChampionKeyArr.Num();
		if (ChampionPool2Cost[key].ChampionKeyArr.Num() <= 0)
			return false;
		ChampionPool2Cost[key].ChampionKeyArr.RemoveAt(arrSize - 1);
		break;
	case 3:
		arrSize = ChampionPool3Cost[key].ChampionKeyArr.Num();
		if (ChampionPool3Cost[key].ChampionKeyArr.Num() <= 0)
			return false;
		ChampionPool3Cost[key].ChampionKeyArr.RemoveAt(arrSize - 1);
		break;
	default:
		break;
	}

	return true;
}

