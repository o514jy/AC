// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AC_TacticianXpInfo.generated.h"

/**
 * 
 */
UCLASS()
class AC_API UAC_TacticianXpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int/*level*/, int/*Max Xp*/> TacticianXpInfoMap;
};
