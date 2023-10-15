// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/AC_UserWidget.h"
#include "AC_HealthBarUI.generated.h"

class UProgressBar;

UCLASS()
class AC_API UAC_HealthBarUI : public UAC_UserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetHealthBar(float xpRatio);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
};
