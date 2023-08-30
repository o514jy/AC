// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AC_Champion.h"
#include "Components/CapsuleComponent.h"

AAC_Champion::AAC_Champion()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Champion"));
}

void AAC_Champion::BeginPlay()
{
	Super::BeginPlay();

	UnHighlightActor();
}

void AAC_Champion::HighlightActor()
{
	bHighlighted = true;

	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);
	Weapon->SetRenderCustomDepth(true);
}

void AAC_Champion::UnHighlightActor()
{
	bHighlighted = false;

	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
