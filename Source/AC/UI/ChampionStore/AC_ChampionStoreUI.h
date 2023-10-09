// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/AC_UserWidget.h"
#include "AC_ChampionCardUI.h"
#include "AC_ChampionStoreUI.generated.h"

class UTextBlock;
class UProgressBar;
class UButton;
class UAC_ChampionCardUI;
class UOverlay;

class AAC_GameMaster;

UCLASS()
class AC_API UAC_ChampionStoreUI : public UAC_UserWidget
{
	GENERATED_BODY()

public:
	UAC_ChampionStoreUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void InitStoreUI();

	UFUNCTION()
	void SetChampionCards();

	UFUNCTION()
	void SetStoreReRoll();

	// set
	UFUNCTION()
	void SetLockedText();

	UFUNCTION()
	void SetbTacticianLevelText(int level);

	UFUNCTION()
	void SetTacticianLevelXpText(int xp);

	UFUNCTION()
	void SetTacticianLevelMaxXpText(int maxXp);

	UFUNCTION()
	void SetTacticianXpProgressBar(float xpRatio);

	UFUNCTION()
	void SetPossessionGoldText(int gold);

	UFUNCTION()
	void SetSellingButtonAndPrice(bool visible, int price);

	UFUNCTION()
	void SetbIsLocked(bool IsLocked);

	UFUNCTION()
	bool GetbIsLocked();

	UFUNCTION()
	bool GetSellingButtonVisible();

private:
	// button event
	UFUNCTION()
	void OnClickedIsLockedButton();

	UFUNCTION()
	void OnClickedBuyXpButton();

	UFUNCTION()
	void OnClickedReRollButton();
	
	//temp
public:
	TObjectPtr<AAC_GameMaster> GetGameMaster();

private:
	UPROPERTY()
	TObjectPtr<AAC_GameMaster> GameMaster;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TacticianLevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TacticianLevelXpText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TacticianLevelMaxXpText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> TacticianXpProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PossessionGoldText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> IsLockedButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LockedText;
	bool bIsLocked = false;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BuyXpButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReRollButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAC_ChampionCardUI> ChampionCard1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAC_ChampionCardUI> ChampionCard2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAC_ChampionCardUI> ChampionCard3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAC_ChampionCardUI> ChampionCard4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAC_ChampionCardUI> ChampionCard5;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SellingButton;
	bool bSellingButtonVisible = false;
	bool sellCondition = false;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SellingPriceText;
};
