// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/AC_LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "AC/Managers/AC_UIManager.h"
#include "AC/Enum/AC_Enum.h"
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Managers/AC_ObjectManager.h"
#include "AC/GameProcessor/AC_GameMaster.h"
#include "AC/Object/AC_EnvObject.h"

AAC_LevelScriptActor::AAC_LevelScriptActor()
{
	
}

TObjectPtr<AAC_GameMaster> AAC_LevelScriptActor::GetGameMaster()
{
	if (GameMaster.IsNull())
	{
		FActorSpawnParameters SpawnParams;
		GameMaster = (AAC_GameMaster*)GetWorld()->SpawnActor<AAC_GameMaster>(AAC_GameMaster::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	}
	return GameMaster;
}

void AAC_LevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	// 초기 카메라 배치
	AActor* camera = UAC_FunctionLibrary::GetObjectManager(GetWorld())->GetEnvObject()->GetMainCamera();
	APlayerController* TacticianPlayController = UGameplayStatics::GetPlayerController(this, 0);
	TacticianPlayController->SetViewTargetWithBlend(camera, 0.f);

	// 초기 UI 생성
	UAC_FunctionLibrary::GetUIManager(GetWorld())->OpenUI(EUIType::ChampionStoreUI);
	GetGameMaster();
	Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->InitStoreUI();
	
	//Cast<UAC_ChampionStoreUI>(UAC_FunctionLibrary::GetUIManager(GetWorld())->GetUI(EUIType::ChampionStoreUI))->SetChampionCards();
	
}
