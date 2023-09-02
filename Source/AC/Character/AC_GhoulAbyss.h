// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AC_Champion.h"
#include "AC_GhoulAbyss.generated.h"

/**
 * 
 */
UCLASS()
class AC_API AAC_GhoulAbyss : public AAC_Champion
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void InitChampionStat() override;
};
