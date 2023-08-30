// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "AC_LevelScriptActor.generated.h"

class AAC_GameMaster;

UCLASS()
class AC_API AAC_LevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	AAC_LevelScriptActor();

	TObjectPtr<AAC_GameMaster> GetGameMaster();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<AAC_GameMaster> GameMaster;
};
