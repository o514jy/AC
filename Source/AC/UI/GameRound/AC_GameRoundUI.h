// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/AC_UserWidget.h"
#include "AC_GameRoundUI.generated.h"

// UI
class UTextBlock;
class UButton;
class UImage;

class AAC_GameMaster;

UCLASS()
class AC_API UAC_GameRoundUI : public UAC_UserWidget
{
	GENERATED_BODY()
	
public:
	UAC_GameRoundUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// set
	UFUNCTION()
	void SetPresentRoundText(int presentRound);

	UFUNCTION()
	void SetOpponentNameText(FString opponentName);

	UFUNCTION()
	void SetGameStateText(EGameState gameState);

	UFUNCTION()
	void SetPresentTeamNumText(int presentTeamNum);

	UFUNCTION()
	void SetMaxTeamNumText(int maxTeamNum);

	UFUNCTION()
	void SetRemainTimeText(int remainTime);

	// temp
public:
	TObjectPtr<AAC_GameMaster> GetGameMaster();

private:
	UPROPERTY()
	TObjectPtr<AAC_GameMaster> GameMaster;

private:
	// UI ¿¬µ¿
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PresentRoundText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OpponentNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GameStateText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PresentTeamNumText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxTeamNumText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RemainTimeText;
};
