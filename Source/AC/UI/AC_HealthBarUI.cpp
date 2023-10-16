// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/AC_HealthBarUI.h"
#include "Components/ProgressBar.h"

void UAC_HealthBarUI::SetHealthBar(float xpRatio)
{
	HealthBar->SetPercent(xpRatio);
}
