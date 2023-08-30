// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AC_GameMaster.generated.h"

struct FChampionInfo;
class AAC_Tactician;

//USTRUCT(BlueprintType)
//struct FChampionKeyQueue
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	TQueue<FString> ChampionKeyQueue;
//};

UCLASS()
class AC_API AAC_GameMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_GameMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FChampionInfo> GetChampionInfoArr();

	void InitChampionPoolAllCost();

	FString PickAtRandomChampionCardName();
	FString FindSpecificName(FRandomStream RS, TMap<FString, int> championPool);

	AAC_Tactician* GetTactician();

	bool SellingChampionForName(FString championName, int championCost);

private:
	UPROPERTY()
	TArray<FChampionInfo> ChampionInfoArr;

	UPROPERTY()
	TObjectPtr<AAC_Tactician> Tactician = nullptr;

	UPROPERTY()
	TMap<FString/*key*/, int/*개수*/> ChampionPool1Cost; // 챔피언당 30

	UPROPERTY()
	TMap<FString/*key*/, int/*개수*/> ChampionPool2Cost; // 챔피언당 15

	UPROPERTY()
	TMap<FString/*key*/, int/*개수*/> ChampionPool3Cost; // 챔피언당 10
};
