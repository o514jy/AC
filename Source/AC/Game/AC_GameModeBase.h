// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AC_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AC_API AAC_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	// Managers
	UFUNCTION(BlueprintCallable)
	class AAC_UIManager* GetUIManager() { return UIManager; }

	UFUNCTION(BlueprintCallable)
	class AAC_GameManager* GetGameManager() { return GameManager; }

	UFUNCTION(BlueprintCallable)
	class AAC_DataManager* GetDataManager() { return DataManager; }

	UFUNCTION(BlueprintCallable)
	class AAC_ObjectManager* GetObjectManager() { return ObjectManager; }
	
private:
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	class AAC_UIManager* UIManager = nullptr;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	class AAC_GameManager* GameManager = nullptr;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	class AAC_DataManager* DataManager = nullptr;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	class AAC_ObjectManager* ObjectManager = nullptr;
};
