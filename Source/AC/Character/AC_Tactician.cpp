﻿// Fill out your copyright notice in the Description page of Project Settings.


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
#include "NiagaraActor.h"

AAC_Tactician::AAC_Tactician()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Tactician"));

	Key = "Tactician1";

	WaitingChampionArr.Init(FString(), 8);
	ArenaChampionArr.Init(FString(), 32);
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
				return;
			}
		}
	}
	else
	{
		SetXp(TacticianStat.Xp + addXp);
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
