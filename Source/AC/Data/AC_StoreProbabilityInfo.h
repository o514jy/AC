// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AC_StoreProbabilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FProbabilityInfoArr
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<int> ProbabilityInfoArr;
};

UCLASS()
class AC_API UAC_StoreProbabilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int/*Tactician Level*/, FProbabilityInfoArr> StoreProbabilityInfoMap;

};
