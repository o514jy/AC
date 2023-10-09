// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProcessor/AC_GameMaster.h"
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Data/AC_ChampionInfo.h"
#include "AC/Data/AC_StoreProbabilityInfo.h"
#include "AC/Character/AC_Tactician.h"
#include "AC/Object/AC_EnvObject.h"
#include "AC/Object/AC_PlaceableObject1x1.h"
#include "AC/Object/AC_ObjectBase.h"
#include "AC/Character/AC_Champion.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "AC/Controller/AC_PlayerController.h"
// Managers
#include "AC/Managers/AC_DataManager.h"
#include "AC/Managers/AC_ObjectManager.h"
#include "AC/Managers/AC_UIManager.h"
#include "AC/Enum/AC_Enum.h"
// UI
#include "AC/UI/GameRound/AC_GameRoundUI.h"

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

	GetWorld()->GetTimerManager().SetTimer(RoundTimer, this, &AAC_GameMaster::SetGame, 1.0f, true, 2.0f);
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
			const FString seatKey = GetTactician()->GetObjectKey() + TEXT("PlaceableObject1x1WaitingSeat") + FString::Printf(TEXT("%d"), i + 1);
			FVector loc = UAC_FunctionLibrary::GetObjectManager(GetWorld())->FindObject(seatKey)->GetActorLocation();
			
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

void AAC_GameMaster::SetGame()
{
	AAC_UIManager* uiManager = UAC_FunctionLibrary::GetUIManager(GetWorld());
	UAC_GameRoundUI* gameRoundUI = Cast<UAC_GameRoundUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::GameRoundUI));
	
	if (bRoundStart == false)
	{
		uiManager->OpenUI(EUIType::GameRoundUI);
		bRoundStart = true;
		SetGameRound(0);
		RoundState = EGameState::Battle;
	}

	SetRemainTimeAndRoundState();

}

void AAC_GameMaster::SetRemainTimeAndRoundState()
{
	UAC_GameRoundUI* gameRoundUI = Cast<UAC_GameRoundUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::GameRoundUI));
	
	// round 상태가 바뀌었을 경우 초기화해주는 부분
	if (RemainTime == -1)
	{
		switch (RoundState)
		{
		case EGameState::Prepare:
			RoundState = EGameState::Ready;
			break;
		case EGameState::Ready:
			RoundState = EGameState::Battle;
			break;
		case EGameState::Battle:
			RoundState = EGameState::Prepare;
			break;
		default:
			break;
		}

		// 바뀐 state를 업데이트
		UpdateRoundState();

	}
	else // round 상태가 진행중인 경우 1초씩 시간 차감해서 반영
	{
		RemainTime--;

		// 시간이 다 되었을 경우 다음 상태로 전환
		if (RemainTime == 1)
		{
			gameRoundUI->SetRemainTimeText(RemainTime); // 1초일때 ui
			RemainTime = -1; // 처음 초기화는 -1로
			return;
		}
	}

	gameRoundUI->SetRemainTimeText(RemainTime);

}

void AAC_GameMaster::UpdateRoundState()
{
	UAC_GameRoundUI* gameRoundUI = Cast<UAC_GameRoundUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::GameRoundUI));

	gameRoundUI->SetGameStateText(RoundState); // 바뀐 State 반영

	switch (RoundState)
	{
	case EGameState::Prepare:
		SetPrepareState();
		break;
	case EGameState::Ready:
		SetReadyState();
		break;
	case EGameState::Battle:
		SetBattleState();
		break;
	default:
		break;
	}
}

void AAC_GameMaster::SetPrepareState()
{
	UAC_GameRoundUI* gameRoundUI = Cast<UAC_GameRoundUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::GameRoundUI));
	UAC_ChampionStoreUI* championStoreUI = Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI));

	AAC_ObjectManager* objManager = UAC_FunctionLibrary::GetObjectManager(GetWorld());

	RemainTime = TimeLimit_Prepare;

	// 라운드 갱신
	SetGameRound(GetGameRound() + 1);

	// 크립 라운드용 팀 초기화
	for (auto& index : CreepTeamKeyArr)
	{
		AAC_Champion* creepChamp = objManager->FindChampion(index);
		if (creepChamp != nullptr)
			creepChamp->Destroy();
	}
	CreepTeamKeyArr.Empty();

	// 상점 새로고침, 잠겨있으면 새로고침 안함
	if (championStoreUI->GetbIsLocked() == true)
		championStoreUI->SetbIsLocked(false);
	else
		championStoreUI->SetStoreReRoll();

	// 전투가 끝났을 경우 팀 다시 위치로 복원


}

void AAC_GameMaster::SetReadyState()
{
	UAC_GameRoundUI* gameRoundUI = Cast<UAC_GameRoundUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::GameRoundUI));

	SpawnCreepTeam(GameRound);

	RemainTime = TimeLimit_Ready;
}

void AAC_GameMaster::SetBattleState()
{
	UAC_GameRoundUI* gameRoundUI = Cast<UAC_GameRoundUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::GameRoundUI));
	
	RemainTime = TimeLimit_Battle;
}

void AAC_GameMaster::SetGameRound(int gameRound)
{
	UAC_GameRoundUI* gameRoundUI = Cast<UAC_GameRoundUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::GameRoundUI));
	gameRoundUI->SetPresentRoundText(gameRound);
	GameRound = gameRound;

}

void AAC_GameMaster::SpawnCreepTeam(int teamNum)
{
	auto* objManager = UAC_FunctionLibrary::GetObjectManager(GetWorld());

	// temp 하드코딩

	FString key1;
	FVector loc1 = objManager->GetEnvObject()->GetArena()[Def_ArenaLeftUp]->GetActorLocation();
	loc1.Z += 60;
	FString key2;
	FVector loc2 = objManager->GetEnvObject()->GetArena()[Def_ArenaLeftUp]->GetActorLocation();
	loc2.Z += 60;
	FString key3;
	FVector loc3 = objManager->GetEnvObject()->GetArena()[Def_ArenaLeftUp]->GetActorLocation();
	loc3.Z += 60;
	FString key4;
	FVector loc4 = objManager->GetEnvObject()->GetArena()[Def_ArenaLeftUp]->GetActorLocation();
	loc4.Z += 60;
	switch (teamNum)
	{
	case 1:
		key1 = "GoblinSpearCreep1";
		loc1.X += 150.f;
		loc1.Y += 350.f;
		objManager->AddAndSpawnCharacter(
			key1,
			loc1,
			FRotator(0, 180, 0),
			FActorSpawnParameters()
		);
		CreepTeamKeyArr.Add(key1);
		key2 = "GoblinSpearCreep2";
		loc2.X += 150.f;
		loc2.Y += 450.f;
		objManager->AddAndSpawnCharacter(
			key2,
			loc2,
			FRotator(0, 180, 0),
			FActorSpawnParameters()
		);
		CreepTeamKeyArr.Add(key2);
		break;
	case 2:
		key1 = "GoblinSpearCreep1";
		loc1.X += 150.f;
		loc1.Y += 350.f;
		objManager->AddAndSpawnCharacter(
			key1,
			loc1,
			FRotator(0, 180, 0),
			FActorSpawnParameters()
		);
		CreepTeamKeyArr.Add(key1);
		key2 = "GoblinSpearCreep2";
		loc2.X += 150.f;
		loc2.Y += 450.f;
		objManager->AddAndSpawnCharacter(
			key2,
			loc2,
			FRotator(0, 180, 0),
			FActorSpawnParameters()
		);
		CreepTeamKeyArr.Add(key2);
		key3 = "GoblinSlingshotCreep1";
		loc3.X += 250.f;
		loc3.Y += 350.f;
		objManager->AddAndSpawnCharacter(
			key3,
			loc3,
			FRotator(0, 180, 0),
			FActorSpawnParameters()
		);
		CreepTeamKeyArr.Add(key3);
		break;
	case 3:
		key1 = "GoblinSpearCreep1";
		loc1.X += 150.f;
		loc1.Y += 350.f;
		objManager->AddAndSpawnCharacter(
			key1,
			loc1,
			FRotator(0, 180, 0),
			FActorSpawnParameters()
		);
		CreepTeamKeyArr.Add(key1);
		key2 = "GoblinSpearCreep2";
		loc2.X += 150.f;
		loc2.Y += 450.f;
		objManager->AddAndSpawnCharacter(
			key2,
			loc2,
			FRotator(0, 180, 0),
			FActorSpawnParameters()
		);
		CreepTeamKeyArr.Add(key2);
		key3 = "GoblinSlingshotCreep1";
		loc3.X += 250.f;
		loc3.Y += 350.f;
		objManager->AddAndSpawnCharacter(
			key3,
			loc3,
			FRotator(0, 180, 0),
			FActorSpawnParameters()
		);
		CreepTeamKeyArr.Add(key3);
		key4 = "GoblinSlingshotCreep2";
		loc4.X += 250.f;
		loc4.Y += 450.f;
		objManager->AddAndSpawnCharacter(
			key4,
			loc4,
			FRotator(0, 180, 0),
			FActorSpawnParameters()
		);
		CreepTeamKeyArr.Add(key4);
		break;
	default:
		break;
	}
}



