// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/AC_ChampionInfo.h"
#include "AC/Managers/AC_DataManager.h"
#include "AC/Library/AC_FunctionLibrary.h"

FChampionInfo UAC_ChampionInfo::FindChampionInfoForKey(const FString& key, bool bLogNotFound)
{
	
	for (const FChampionInfo& Info : ChampionInfoArr)
	{
		if (Info.Key == key)
		{
			return Info;
		}
	}

	if (bLogNotFound)
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for name [%s]"), *key);

	return FChampionInfo();
}
