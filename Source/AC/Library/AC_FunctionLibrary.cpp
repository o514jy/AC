// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/AC_FunctionLibrary.h"
#include "AC/Game/AC_GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "AC/Managers/AC_UIManager.h"
#include "AC/Managers/AC_GameManager.h"
#include "AC/Managers/AC_DataManager.h"
#include "AC/Managers/AC_ObjectManager.h"


AAC_UIManager* UAC_FunctionLibrary::GetUIManager(UWorld* world)
{
	AAC_GameModeBase* GM = Cast<AAC_GameModeBase>(UGameplayStatics::GetGameMode(world));
	return GM->GetUIManager();
}

AAC_GameManager* UAC_FunctionLibrary::GetGameManager(UWorld* world)
{
	AAC_GameModeBase* GM = Cast<AAC_GameModeBase>(UGameplayStatics::GetGameMode(world));
	return GM->GetGameManager();
}

AAC_DataManager* UAC_FunctionLibrary::GetDataManager(UWorld* world)
{
	AAC_GameModeBase* GM = Cast<AAC_GameModeBase>(UGameplayStatics::GetGameMode(world));
	return GM->GetDataManager();
}

AAC_ObjectManager* UAC_FunctionLibrary::GetObjectManager(UWorld* world)
{
	AAC_GameModeBase* GM = Cast<AAC_GameModeBase>(UGameplayStatics::GetGameMode(world));
	return GM->GetObjectManager();
}


