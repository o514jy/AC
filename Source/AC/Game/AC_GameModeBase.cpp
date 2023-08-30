// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AC_GameModeBase.h"
#include "AC/Managers/AC_UIManager.h"
#include "AC/Managers/AC_GameManager.h"
#include "AC/Managers/AC_DataManager.h"
#include "AC/Managers/AC_ObjectManager.h"

void AAC_GameModeBase::BeginPlay()
{
	FActorSpawnParameters SpawnParams;

	UIManager = (AAC_UIManager*)GetWorld()->SpawnActor<AAC_UIManager>(AAC_UIManager::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	GameManager = (AAC_GameManager*)GetWorld()->SpawnActor<AAC_GameManager>(AAC_GameManager::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	DataManager = (AAC_DataManager*)GetWorld()->SpawnActor<AAC_DataManager>(AAC_DataManager::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	ObjectManager = (AAC_ObjectManager*)GetWorld()->SpawnActor<AAC_ObjectManager>(AAC_ObjectManager::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
}
