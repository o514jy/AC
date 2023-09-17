// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../UI/AC_UserWidget.h"
#include "../UI/ChampionStore/AC_ChampionCardUI.h"
#include "../UI/ChampionStore/AC_ChampionStoreUI.h"
#include "../UI/GameRound/AC_GameRoundUI.h"
#include "../Enum/AC_Enum.h"
#include "AC_UIManager.generated.h"

class UAC_ChampionCardUI;
class UAC_ChampionStoreUI;
class UAC_GameRoundUI;

UCLASS()
class AC_API AAC_UIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_UIManager(); 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenUI(EUIType type);

	UAC_UserWidget* GetUI(EUIType type);

private:
	UPROPERTY()
	TObjectPtr<UAC_UserWidget> ChampionStoreUI = nullptr;

	UPROPERTY()
	TSubclassOf<UAC_UserWidget> ChampionStoreUIClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAC_UserWidget> GameRoundUI = nullptr;

	UPROPERTY()
	TSubclassOf<UAC_UserWidget> GameRoundUIClass = nullptr;
	
};
