// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AC_DataManager.h"
#include "AC/Data/AC_ChampionInfo.h"
#include "AC/Library/AC_FunctionLibrary.h"

// Sets default values
AAC_DataManager::AAC_DataManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (ChampionInfo == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<UDataAsset> DataAsset(TEXT("AC_ChampionInfo'/Game/Blueprints/Data/DA_ChampionInfo.DA_ChampionInfo'"));
		if (DataAsset.Succeeded())
		{
			UDataAsset* dataAsset = DataAsset.Object;
			ChampionInfo = Cast<UAC_ChampionInfo>(dataAsset);
		}
	}

	if (StoreProbabilityInfo == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<UDataAsset> DataAsset(TEXT("AC_StoreProbabilityInfo'/Game/Blueprints/Data/DA_StoreProbabilityInfo.DA_StoreProbabilityInfo'"));
		if (DataAsset.Succeeded())
		{
			UDataAsset* dataAsset = DataAsset.Object;
			StoreProbabilityInfo = Cast<UAC_StoreProbabilityInfo>(dataAsset);
		}
	}
	
	if (TacticianXpInfo == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<UDataAsset> DataAsset(TEXT("AC_TacticianXpInfo'/Game/Blueprints/Data/DA_TacticianXpInfo.DA_TacticianXpInfo'"));
		if (DataAsset.Succeeded())
		{
			UDataAsset* dataAsset = DataAsset.Object;
			TacticianXpInfo = Cast<UAC_TacticianXpInfo>(dataAsset);
		}
	}
}

// Called when the game starts or when spawned
void AAC_DataManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAC_DataManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAC_ChampionInfo* AAC_DataManager::GetChampionInfo()
{
	return ChampionInfo;
}

UAC_StoreProbabilityInfo* AAC_DataManager::GetStoreProbabilityInfo()
{
	return StoreProbabilityInfo;
}

UAC_TacticianXpInfo* AAC_DataManager::GetTacticianXpInfo()
{
	return TacticianXpInfo;
}

