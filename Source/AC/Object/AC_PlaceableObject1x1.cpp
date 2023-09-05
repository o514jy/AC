// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/AC_PlaceableObject1x1.h"
#include "Components/BoxComponent.h"

AAC_PlaceableObject1x1::AAC_PlaceableObject1x1()
{
	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
}

void AAC_PlaceableObject1x1::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AAC_PlaceableObject1x1::OnBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AAC_PlaceableObject1x1::OnEndOverlap);
}

void AAC_PlaceableObject1x1::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsOverlapped = true;
}

void AAC_PlaceableObject1x1::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlapped = false;
}
