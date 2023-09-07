// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProcessor/AC_GameMaster.h"
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Managers/AC_DataManager.h"
#include "AC/Managers/AC_ObjectManager.h"
#include "AC/Data/AC_ChampionInfo.h"
#include "AC/Data/AC_StoreProbabilityInfo.h"
#include "AC/Character/AC_Tactician.h"
#include "AC/Object/AC_EnvObject.h"
#include "AC/Object/AC_PlaceableObject1x1.h"
#include "AC/Object/AC_ObjectBase.h"
#include "AC/Character/AC_Champion.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "AC/Enum/AC_Enum.h"
#include "AC/Controller/AC_PlayerController.h"

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

	AAC_PlayerController* PC = Cast<AAC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PC->ResellToStore.BindUObject(this, &AAC_GameMaster::ResellChampionCardUsingKey);
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
		Tactician->SetObjectKey(TEXT("Tactician") + FString::Printf(TEXT("%d"), 0));
	}
	return Tactician;
}

void AAC_GameMaster::ResellChampionCardUsingKey(const FString& key)
{
	AAC_ObjectManager* OM = UAC_FunctionLibrary::GetObjectManager(GetWorld());
	
	GetTactician()->AddPossessionGold(OM->FindChampion(key)->GetChampionStat().ChampionCost);

	TArray<FString> wcarr = GetTactician()->GetWaitingChampionArr();
	for (int i = 0; i < wcarr.Num(); i++)
	{
		if (wcarr[i] == key)
		{
			GetTactician()->SetWaitingChampionArr(FString(), i);
			break;
		}
	}


	for (auto& index : ChampionInfoArr)
	{
		if (key.Contains(index.Key))
		{
			switch (index.ChampionCost)
			{
			case 1:
				ChampionPool1Cost[index.Key].ChampionKeyArr.Add(key);
				break;
			case 2:
				ChampionPool2Cost[index.Key].ChampionKeyArr.Add(key);
				break;
			case 3:
				ChampionPool3Cost[index.Key].ChampionKeyArr.Add(key);
				break;
			default:
				break;
			}
			OM->DestroyChampion(key);
		}
	}
}

bool AAC_GameMaster::SellChampionCardUsingKey(FString key, int championCost)
{
	bool sellPossibility = false;

	switch (championCost)
	{
	case 1:
		sellPossibility = FindAndPlaceEmptySeat(key, OUT ChampionPool1Cost);
		break;
	case 2:
		sellPossibility = FindAndPlaceEmptySeat(key, OUT ChampionPool2Cost);
		break;
	case 3:
		sellPossibility = FindAndPlaceEmptySeat(key, OUT ChampionPool3Cost);
		break;
	default:
		break;
	}

	return sellPossibility;
}

bool AAC_GameMaster::FindAndPlaceEmptySeat(FString key, TMap<FString, FChampionKeyArr>& championPool)
{
	int arrSize;
	TArray<FString> wcarr;
	bool sold = false;

	arrSize = championPool[key].ChampionKeyArr.Num();
	if (arrSize <= 0)
		return false;
	FString inGameKey = championPool[key].ChampionKeyArr[arrSize - 1];
	championPool[key].ChampionKeyArr.RemoveAt(arrSize - 1);
	// TODO : 스폰
	wcarr = GetTactician()->GetWaitingChampionArr();
	for (int i = 0; i < wcarr.Num(); i++)
	{
		if (wcarr[i] == FString())
		{
			sold = true;
			GetTactician()->SetWaitingChampionArr(inGameKey, i);
			const FString key = GetTactician()->GetObjectKey() + TEXT("PlaceableObject1x1WaitingSeat") + FString::Printf(TEXT("%d"), i + 1);
			FVector loc = UAC_FunctionLibrary::GetObjectManager(GetWorld())->FindObject(key)->GetActorLocation();
			
			UAC_FunctionLibrary::GetObjectManager(GetWorld())->AddAndSpawnCharacter(
				inGameKey,
				loc,
				FRotator(0, 0, 0),
				FActorSpawnParameters()
			);
			break;
		}
	}
	return sold;
}

