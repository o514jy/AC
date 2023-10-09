// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChampionStore/AC_ChampionCardUI.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "AC/Data/AC_ChampionInfo.h"
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Managers/AC_DataManager.h"
#include "Kismet/GameplayStatics.h"
#include "AC/GameProcessor/AC_GameMaster.h"
#include "AC/Character/AC_Tactician.h"
#include "Kismet/KismetStringLibrary.h"

UAC_ChampionCardUI::UAC_ChampionCardUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UAC_ChampionCardUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (BuyChampionButton)
		BuyChampionButton->OnClicked.AddDynamic(this, &UAC_ChampionCardUI::OnClickedBuyChampionButton);
}

void UAC_ChampionCardUI::SetChampionCardUI(FString key)
{
	FChampionInfo info = UAC_FunctionLibrary::GetDataManager(GetWorld())->GetChampionInfo()->FindChampionInfoForKey(key);
	Key = info.Key;
	EthnicSynergyText->SetText(FText::FromString(info.Ethnic));
	OccupationSynergyText->SetText(FText::FromString(info.Occupation));
	ChampionNameText->SetText(FText::FromString(info.ChampionName));
	ChampionCostText->SetText(FText::FromString(FString::FromInt(info.ChampionCost)));
}

void UAC_ChampionCardUI::SetbSold(bool sold)
{
	if (sold == true)
	{
		SetVisibility(ESlateVisibility::Hidden);
		bSold = true;
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
		bSold = false;
	}
}

void UAC_ChampionCardUI::OnClickedBuyChampionButton()
{
	int ChampionCost = UKismetStringLibrary::Conv_StringToInt(ChampionCostText->GetText().ToString());
	if (GetGameMaster()->SellChampionCardUsingKey(Key, ChampionCost) == true && GetGameMaster()->GetTactician()->SubPossessionGold(ChampionCost) == true)
	{
		SetbSold(true);
	}
}

TObjectPtr<AAC_GameMaster> UAC_ChampionCardUI::GetGameMaster()
{
	TArray<AActor*> arrOutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAC_GameMaster::StaticClass(), arrOutActors);

	for (int i = 0; i < arrOutActors.Num(); ++i)
	{
		GameMaster = dynamic_cast<AAC_GameMaster*>(arrOutActors[i]);
	}

	return GameMaster;
}


