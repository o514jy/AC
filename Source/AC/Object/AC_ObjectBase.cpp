// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/AC_ObjectBase.h"

// Sets default values
AAC_ObjectBase::AAC_ObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAC_ObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAC_ObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

