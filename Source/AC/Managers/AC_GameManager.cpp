// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AC_GameManager.h"

// Sets default values
AAC_GameManager::AAC_GameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAC_GameManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAC_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
