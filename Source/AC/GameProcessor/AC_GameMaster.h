// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AC_GameMaster.generated.h"

struct FChampionInfo;
class AAC_Tactician;
class IAC_TargetInterface;

USTRUCT(BlueprintType)
struct FChampionKeyArr
{
	GENERATED_BODY()

public:
	TArray<FString> ChampionKeyArr;
};

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
	FString PickChampionCardRandomlyUsingKey();
	FString FindSpecificKey(FRandomStream RS, TMap<FString, FChampionKeyArr> championPool);

	AAC_Tactician* GetTactician();

	UFUNCTION()
	void ResellChampionCardUsingKey(const FString& key);
	
	bool SellChampionCardUsingKey(FString key, int championCost);
	bool FindAndPlaceEmptySeat(FString key, TMap<FString, FChampionKeyArr>& championPool);

private:
	UPROPERTY()
	TArray<FChampionInfo> ChampionInfoArr;

	UPROPERTY()
	TObjectPtr<AAC_Tactician> Tactician = nullptr;

	UPROPERTY()
	TMap<FString/*key*/, FChampionKeyArr> ChampionPool1Cost; // 챔피언당 30

	UPROPERTY()
	TMap<FString/*key*/, FChampionKeyArr> ChampionPool2Cost; // 챔피언당 15

	UPROPERTY()
	TMap<FString/*key*/, FChampionKeyArr> ChampionPool3Cost; // 챔피언당 10
};
