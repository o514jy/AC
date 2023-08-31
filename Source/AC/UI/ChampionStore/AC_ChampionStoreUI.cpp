// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChampionStore/AC_ChampionStoreUI.h"
#include "AC/UI/ChampionStore/AC_ChampionCardUI.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"

#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Managers/AC_DataManager.h"
#include "AC/Data/AC_TacticianXpInfo.h"
#include "AC/GameProcessor/AC_GameMaster.h"
#include "AC/Character/AC_Tactician.h"
#include "Kismet/GameplayStatics.h"
#include "AC/Enum/AC_Enum.h"

UAC_ChampionStoreUI::UAC_ChampionStoreUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UAC_ChampionStoreUI::NativeConstruct()
{
	Super::NativeConstruct();
	SetbIsLocked(false);
	
	if (IsLockedButton)
		IsLockedButton->OnClicked.AddDynamic(this, &UAC_ChampionStoreUI::OnClickedIsLockedButton);

	if (ReRollButton)
		ReRollButton->OnClicked.AddDynamic(this, &UAC_ChampionStoreUI::OnClickedReRollButton);

	if (BuyXpButton)
		BuyXpButton->OnClicked.AddDynamic(this, &UAC_ChampionStoreUI::OnClickedBuyXpButton);
}

void UAC_ChampionStoreUI::InitStoreUI()
{
	SetChampionCards();

	FTacticianStat tacticianStat = GetGameMaster()->GetTactician()->GetTacticianStat();

	int currLevel = tacticianStat.Level;
	SetbTacticianLevelText(currLevel);

	int currXp = tacticianStat.Xp;
	int maxXp = *UAC_FunctionLibrary::GetDataManager(GetWorld())->GetTacticianXpInfo()->TacticianXpInfoMap.Find(currLevel);
	SetTacticianLevelXpText(currXp);
	SetTacticianLevelMaxXpText(maxXp);

	float xpRatio;
	if (maxXp == 0)
		xpRatio = 0;
	else
		xpRatio = currXp / (float)maxXp;
	SetTacticianXpProgressBar(xpRatio);

	int possessionGold = tacticianStat.PossessionGold;
	SetPossessionGoldText(possessionGold);


}

void UAC_ChampionStoreUI::SetChampionCards()
{
	ChampionCard1->SetChampionCardUI(GetGameMaster()->PickChampionCardRandomlyUsingKey());
	ChampionCard2->SetChampionCardUI(GetGameMaster()->PickChampionCardRandomlyUsingKey());
	ChampionCard3->SetChampionCardUI(GetGameMaster()->PickChampionCardRandomlyUsingKey());
	ChampionCard4->SetChampionCardUI(GetGameMaster()->PickChampionCardRandomlyUsingKey());
	ChampionCard5->SetChampionCardUI(GetGameMaster()->PickChampionCardRandomlyUsingKey());
}

void UAC_ChampionStoreUI::OnClickedIsLockedButton()
{
	if (bIsLocked == true)
	{
		SetbIsLocked(false);
	}
	else
	{
		SetbIsLocked(true);
	}
}

void UAC_ChampionStoreUI::OnClickedBuyXpButton()
{
	if (GetGameMaster()->GetTactician()->GetTacticianStat().Level >= NormalMaxLevel)
		return;

	if(GetGameMaster()->GetTactician()->SubPossessionGold(4))
		GetGameMaster()->GetTactician()->addXp(4);

}

void UAC_ChampionStoreUI::OnClickedReRollButton()
{
	
	if (GetGameMaster()->GetTactician()->SubPossessionGold(2))
	{
		SetbIsLocked(false);
		SetChampionCards();
		ChampionCard1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ChampionCard2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ChampionCard3->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ChampionCard4->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ChampionCard5->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UAC_ChampionStoreUI::SetbIsLocked(bool IsLocked)
{
	bIsLocked = IsLocked;
	SetLockedText();
}

TObjectPtr<AAC_GameMaster> UAC_ChampionStoreUI::GetGameMaster()
{
	TArray<AActor*> arrOutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAC_GameMaster::StaticClass(), arrOutActors);

	for (int i = 0; i < arrOutActors.Num(); ++i)
	{
		GameMaster = dynamic_cast<AAC_GameMaster*>(arrOutActors[i]);
	}

	return GameMaster;
}

void UAC_ChampionStoreUI::SetLockedText()
{
	if (bIsLocked == true)
	{
		LockedText->SetText(FText::FromString("Locked"));
	}
	else
	{
		LockedText->SetText(FText::FromString("UnLocked"));
	}
}

void UAC_ChampionStoreUI::SetbTacticianLevelText(int level)
{
	TacticianLevelText->SetText(FText::FromString(FString::FromInt(level)));
}

void UAC_ChampionStoreUI::SetTacticianLevelXpText(int xp)
{
	TacticianLevelXpText->SetText(FText::FromString(FString::FromInt(xp)));
}

void UAC_ChampionStoreUI::SetTacticianLevelMaxXpText(int maxXp)
{
	TacticianLevelMaxXpText->SetText(FText::FromString(FString::FromInt(maxXp)));
}

void UAC_ChampionStoreUI::SetTacticianXpProgressBar(float xpRatio)
{
	TacticianXpProgressBar->SetPercent(xpRatio);
}

void UAC_ChampionStoreUI::SetPossessionGoldText(int gold)
{
	PossessionGoldText->SetText(FText::FromString(FString::FromInt(gold)));
}

bool UAC_ChampionStoreUI::GetbIsLocked()
{
	return bIsLocked;
}
