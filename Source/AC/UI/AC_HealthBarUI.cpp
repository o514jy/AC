// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ProgressBar.h"
#include "UI/AC_HealthBarUI.h"

void UAC_HealthBarUI::SetHealthBar(float xpRatio)
{
	HealthBar->SetPercent(xpRatio);
}
