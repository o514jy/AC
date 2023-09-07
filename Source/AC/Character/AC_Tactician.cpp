// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AC_Tactician.h"
#include "Components/CapsuleComponent.h"
#include "AC/Data/AC_TacticianXpInfo.h"
#include "AC/Enum/AC_Enum.h"
// Manager
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Managers/AC_UIManager.h"
#include "AC/Managers/AC_DataManager.h"
#include "AC/Managers/AC_ObjectManager.h"
// Object & Niagara
#include "AC/Object/AC_EnvObject.h"
#include "Engine/StaticMeshActor.h"
#include "NiagaraActor.h"
// Placeable
#include "AC/Object/AC_PlaceableObject1x1.h"


AAC_Tactician::AAC_Tactician()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Tactician"));

	Key = "Tactician1";

	WaitingChampionArr.Init(FString(), 8);
	ArenaChampionArr.Init(FArenaChampionRowArr(), 4);
}

void AAC_Tactician::BeginPlay()
{
	Super::BeginPlay();

	GetPlaceableWaitingSeat();
	GetPlaceableArena();
}

FTacticianStat AAC_Tactician::GetTacticianStat()
{
	return TacticianStat;
}

void AAC_Tactician::SetLevel(int level)
{
	TacticianStat.Level = level;
	Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetbTacticianLevelText(level);
}

void AAC_Tactician::SetXp(int xp)
{
	TacticianStat.Xp = xp;
	Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetTacticianLevelXpText(xp);
}

void AAC_Tactician::SetMaxXp(int currLevel)
{
	TMap<int, int> maxXpInfo = UAC_FunctionLibrary::GetDataManager(GetWorld())->GetTacticianXpInfo()->TacticianXpInfoMap;
	int maxXp = *maxXpInfo.Find(currLevel);
	TacticianStat.maxXp = maxXp;
	Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetTacticianLevelMaxXpText(maxXp);
}

void AAC_Tactician::addXp(int addXp)
{
	TMap<int, int> maxXpInfo = UAC_FunctionLibrary::GetDataManager(GetWorld())->GetTacticianXpInfo()->TacticianXpInfoMap;
	
	int currMaxXp = *maxXpInfo.Find(TacticianStat.Level);
	int currLevel = TacticianStat.Level;

	if (TacticianStat.Xp + addXp >= currMaxXp)
	{
		int restXp = TacticianStat.Xp + addXp - currMaxXp;

		while (true)
		{
			int nextLevel = currLevel + 1;

			// 다음 레벨이 최대레벨이라면?
			if (nextLevel == NormalMaxLevel)
			{
				SetXp(0);
				SetLevel(nextLevel);
				SetMaxXp(1);
				Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetTacticianXpProgressBar((float)TacticianStat.Xp / TacticianStat.maxXp);
				return;
			}
			
			int nextMaxXp = *maxXpInfo.Find(nextLevel);

			if (restXp >= nextMaxXp)
			{
				// 경험치가 다음 경험치도 초과할 경우 한번 더 반복
				restXp -= nextMaxXp;
				currLevel++;
			}
			else
			{
				SetXp(restXp);
				SetLevel(nextLevel);
				SetMaxXp(nextLevel);
				Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetTacticianXpProgressBar((float)TacticianStat.Xp / TacticianStat.maxXp);
				return;
			}
		}
	}
	else
	{
		SetXp(TacticianStat.Xp + addXp);
		Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetTacticianXpProgressBar((float)TacticianStat.Xp / TacticianStat.maxXp);
	}
}

void AAC_Tactician::SetPossessionGold(int gold)
{
	TacticianStat.PossessionGold = gold;
	Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetPossessionGoldText(gold);

	ActivateInterestFire();
}

void AAC_Tactician::AddPossessionGold(int addGold)
{
	TacticianStat.PossessionGold += addGold;
	Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetPossessionGoldText(TacticianStat.PossessionGold);
	
	ActivateInterestFire();
}

bool AAC_Tactician::SubPossessionGold(int subGold)
{
	if (TacticianStat.PossessionGold - subGold < 0)
		return false;

	TacticianStat.PossessionGold -= subGold;
	Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetPossessionGoldText(TacticianStat.PossessionGold);

	ActivateInterestFire();

	return true;
}

void AAC_Tactician::SetWaitingChampionArr(const FString& key, int index)
{
	WaitingChampionArr[index] = key;
}

TArray<FString> AAC_Tactician::GetWaitingChampionArr()
{
	return WaitingChampionArr;
}

void AAC_Tactician::SetArenaChampionArr(const FString& key, int i_index, int j_index)
{
	ArenaChampionArr[i_index].ArenaChampionRowArr[j_index] = key;
}

TArray<FArenaChampionRowArr> AAC_Tactician::GetArenaChampionArr()
{
	return ArenaChampionArr;
}

TArray<TObjectPtr<AAC_PlaceableObject1x1>> AAC_Tactician::GetPlaceableWaitingSeat()
{
	if (PlaceableWaitingSeat.IsEmpty() == true)
	{
		PlaceableWaitingSeat.Init(nullptr, 8);
		for (int i = 0; i < 8; i++)
		{
			const FString key = GetObjectKey() + TEXT("PlaceableObject1x1WaitingSeat") + FString::Printf(TEXT("%d"), i + 1);
			FVector loc = UAC_FunctionLibrary::GetObjectManager(GetWorld())->GetEnvObject()->GetWaitingSeat()[Def_WaitingSeat1234]->GetActorLocation();
			loc.X += 50.f;
			loc.Y += 50.f + i * 100.f;
			loc.Z += 12.5f;
			UAC_FunctionLibrary::GetObjectManager(GetWorld())->AddAndSpawnObject(key, loc, FRotator(), FActorSpawnParameters());
			UAC_FunctionLibrary::GetObjectManager(GetWorld())->SetObjectOnOff(key, false);
			PlaceableWaitingSeat[i] = Cast<AAC_PlaceableObject1x1>(UAC_FunctionLibrary::GetObjectManager(GetWorld())->FindObject(key));
		}
	}
	return PlaceableWaitingSeat;
}

TArray<FPlaceableArenaRowArr> AAC_Tactician::GetPlaceableArena()
{
	if (PlaceableArena.IsEmpty() == true)
	{
		PlaceableArena.Init(FPlaceableArenaRowArr(), 4);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				const FString key = GetObjectKey() + TEXT("PlaceableObject1x1Arena") + FString::Printf(TEXT("%d"), i + 1) + FString::Printf(TEXT("%d"), j + 1);
				FVector loc = UAC_FunctionLibrary::GetObjectManager(GetWorld())->GetEnvObject()->GetArena()[Def_ArenaLeftUp]->GetActorLocation();
				loc.X += -50.f - i * 100.f;
				loc.Y += 50.f + j * 100.f;
				loc.Z += 12.5f;
				UAC_FunctionLibrary::GetObjectManager(GetWorld())->AddAndSpawnObject(key, loc, FRotator(), FActorSpawnParameters());
				UAC_FunctionLibrary::GetObjectManager(GetWorld())->SetObjectOnOff(key, false);
				PlaceableArena[i].RowPlaceableArenaArr[j] = Cast<AAC_PlaceableObject1x1>(UAC_FunctionLibrary::GetObjectManager(GetWorld())->FindObject(key));
			}
		}
	}

	return PlaceableArena;
}

void AAC_Tactician::ActivateInterestFire()
{
	AAC_EnvObject* envObject = UAC_FunctionLibrary::GetObjectManager(GetWorld())->GetEnvObject();
	TArray<ANiagaraActor*> fireArr = envObject->GetFire();
	
	if (TacticianStat.PossessionGold >= 10)
		fireArr[Def_NS_Fire1]->SetActorHiddenInGame(false);
	else
		fireArr[Def_NS_Fire1]->SetActorHiddenInGame(true);

	if (TacticianStat.PossessionGold >= 20)
		fireArr[Def_NS_Fire2]->SetActorHiddenInGame(false);
	else
		fireArr[Def_NS_Fire2]->SetActorHiddenInGame(true);

	if (TacticianStat.PossessionGold >= 30)
		fireArr[Def_NS_Fire3]->SetActorHiddenInGame(false);
	else
		fireArr[Def_NS_Fire3]->SetActorHiddenInGame(true);

	if (TacticianStat.PossessionGold >= 40)
		fireArr[Def_NS_Fire4]->SetActorHiddenInGame(false);
	else
		fireArr[Def_NS_Fire4]->SetActorHiddenInGame(true);

	if (TacticianStat.PossessionGold >= 50)
		fireArr[Def_NS_Fire5]->SetActorHiddenInGame(false);
	else
		fireArr[Def_NS_Fire5]->SetActorHiddenInGame(true);
}
