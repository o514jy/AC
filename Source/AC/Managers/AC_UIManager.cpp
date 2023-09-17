// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AC_UIManager.h"

// Sets default values
AAC_UIManager::AAC_UIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (ChampionStoreUIClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<UAC_ChampionStoreUI> StoreUI(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ChampionStore/WBP_ChampionStore.WBP_ChampionStore_C'"));
		if (StoreUI.Succeeded())
		{
			ChampionStoreUIClass = StoreUI.Class;
		}
	}

	if (GameRoundUIClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<UAC_GameRoundUI> RoundUI(TEXT("WidgetBlueprint'/Game/Blueprints/UI/GameRound/WBP_GameRound.WBP_GameRound_C'"));
		if (RoundUI.Succeeded())
		{
			GameRoundUIClass = RoundUI.Class;
		}
	}
}

// Called when the game starts or when spawned
void AAC_UIManager::BeginPlay()
{
	Super::BeginPlay();
	
	//ChampionStoreUI = GetWorld()->SpawnActor<UAC_ChampionStoreUI>(ChampionStoreUIClass);
	ChampionStoreUI = CreateWidget<UAC_UserWidget>(GetWorld(), ChampionStoreUIClass);
	GameRoundUI = CreateWidget<UAC_UserWidget>(GetWorld(), GameRoundUIClass);
}

// Called every frame
void AAC_UIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAC_UIManager::OpenUI(EUIType type)
{
	switch (type)
	{
	case EUIType::ChampionCardUI:
		break;
	case EUIType::ChampionStoreUI:
		ChampionStoreUI->AddToViewport();
		break;
	case EUIType::GameRoundUI:
		GameRoundUI->AddToViewport();
	default:
		break;
	}
}

UAC_UserWidget* AAC_UIManager::GetUI(EUIType type)
{
	switch (type)
	{
	case EUIType::ChampionCardUI:
		break;
	case EUIType::ChampionStoreUI:
		return ChampionStoreUI;
		break;
	case EUIType::GameRoundUI:
		return GameRoundUI;
	default:
		break;
	}

	return nullptr;
}

