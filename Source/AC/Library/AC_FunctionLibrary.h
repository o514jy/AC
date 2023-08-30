// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AC_FunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AC_API UAC_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	static class AAC_UIManager* GetUIManager(UWorld* world);

	UFUNCTION(BlueprintPure)
	static class AAC_GameManager* GetGameManager(UWorld* world);

	UFUNCTION(BlueprintPure)
	static class AAC_DataManager* GetDataManager(UWorld* world);

	UFUNCTION(BlueprintPure)
	static class AAC_ObjectManager* GetObjectManager(UWorld* world);
};
