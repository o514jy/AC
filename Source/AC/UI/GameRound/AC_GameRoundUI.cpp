// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameRound/AC_GameRoundUI.h"
#include "AC/GameProcessor/AC_GameMaster.h"
#include "Kismet/GameplayStatics.h"
#include "AC/Enum/AC_Enum.h"
#include "Components/TextBlock.h"


UAC_GameRoundUI::UAC_GameRoundUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UAC_GameRoundUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAC_GameRoundUI::SetPresentRoundText(int presentRound)
{
	PresentRoundText->SetText(FText::FromString(FString::FromInt(presentRound)));
}

void UAC_GameRoundUI::SetOpponentNameText(FString opponentName)
{
	OpponentNameText->SetText(FText::FromString(opponentName));
}

void UAC_GameRoundUI::SetGameStateText(EGameState gameState)
{
	switch (gameState)
	{
	case EGameState::Prepare:
		GameStateText->SetText(FText::FromString("Prepare"));
		break;
	case EGameState::Ready:
		GameStateText->SetText(FText::FromString("Ready"));
		break;
	case EGameState::Battle:
		GameStateText->SetText(FText::FromString("Battle"));
		break;
	}
}

void UAC_GameRoundUI::SetPresentTeamNumText(int presentTeamNum)
{
	PresentTeamNumText->SetText(FText::FromString(FString::FromInt(presentTeamNum)));
}

void UAC_GameRoundUI::SetMaxTeamNumText(int maxTeamNum)
{
	MaxTeamNumText->SetText(FText::FromString(FString::FromInt(maxTeamNum)));
}

void UAC_GameRoundUI::SetRemainTimeText(int remainTime)
{
	RemainTimeText->SetText(FText::FromString(FString::FromInt(remainTime)));
}

TObjectPtr<AAC_GameMaster> UAC_GameRoundUI::GetGameMaster()
{
	TArray<AActor*> arrOutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAC_GameMaster::StaticClass(), arrOutActors);

	for (int i = 0; i < arrOutActors.Num(); ++i)
	{
		GameMaster = dynamic_cast<AAC_GameMaster*>(arrOutActors[i]);
	}

	return GameMaster;
}
