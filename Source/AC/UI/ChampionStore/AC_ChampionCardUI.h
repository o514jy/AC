// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/AC_UserWidget.h"
#include "AC_ChampionCardUI.generated.h"

// UI
class UTextBlock;
class UButton;
class UImage;

class AAC_GameMaster;

UCLASS()
class AC_API UAC_ChampionCardUI : public UAC_UserWidget
{
	GENERATED_BODY()
		
public:
	UAC_ChampionCardUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	void SetChampionCardUI(FString championName);

private:
	// button event
	UFUNCTION()
	void OnClickedBuyChampionButton();

	//temp
public:
	TObjectPtr<AAC_GameMaster> GetGameMaster();

private:
	UPROPERTY()
	TObjectPtr<AAC_GameMaster> GameMaster;

private:
	// UI 연동
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BuyChampionButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ChampionImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EthnicSynergyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OccupationSynergyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ChampionNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ChampionRarityImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ChampionCostText;
};
