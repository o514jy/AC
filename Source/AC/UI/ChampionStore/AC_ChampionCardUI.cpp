// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChampionStore/AC_ChampionCardUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "AC/Data/AC_ChampionInfo.h"
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Managers/AC_DataManager.h"
#include "Kismet/GameplayStatics.h"
#include "AC/GameProcessor/AC_GameMaster.h"
#include "AC/Character/AC_Tactician.h"
#include "Kismet/KismetStringLibrary.h"
#include "Engine/Texture2D.h"
#include "Styling/SlateBrush.h"

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

	UTexture2D* MyTexture = GetImagePath(key);
	if (MyTexture)
	{
		FSlateBrush Brush;
		Brush.SetImageSize(FVector2D(200.f, 95.f));
		Brush.SetResourceObject(MyTexture);
		ChampionImage->SetBrush(Brush);
	}
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

UTexture2D* UAC_ChampionCardUI::GetImagePath(FString key)
{
	UTexture2D* LoadedTexture;
	FString ImagePath;

	if (key.Contains(TEXT("DemonDark")))
		ImagePath = "/Script/Engine.Texture2D'/Game/Assets/Enemies/Demon/DemonDark.DemonDark'";

	if (key.Contains(TEXT("DemonRed")))
		ImagePath = "/Script/Engine.Texture2D'/Game/Assets/Enemies/Demon/DemonRed.DemonRed'";

	if (key.Contains(TEXT("GhoulAbyss")))
		ImagePath = "/Script/Engine.Texture2D'/Game/Assets/Enemies/Ghoul/GhoulAbyss.GhoulAbyss'";

	if (key.Contains(TEXT("GoblinShaman")))
		ImagePath = "/Script/Engine.Texture2D'/Game/Assets/Enemies/Shaman/GoblinShaman.GoblinShaman'";

	if (key.Contains(TEXT("GoblinSlingshot")))
		ImagePath = "/Script/Engine.Texture2D'/Game/Assets/Enemies/Goblin/GoblinSlingshot.GoblinSlingshot'";

	if (key.Contains(TEXT("GoblinSpear")))
		ImagePath = "/Script/Engine.Texture2D'/Game/Assets/Enemies/Goblin/GoblinSpear.GoblinSpear'";

	if (key.Contains(TEXT("ShroomPoison")))
		ImagePath = "Texture2D'/Game/Assets/Enemies/Shroom/ShroomPoison.ShroomPoison'";

	LoadedTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *ImagePath));
	if (LoadedTexture)
	{
		return LoadedTexture;
	}

	return nullptr;
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


