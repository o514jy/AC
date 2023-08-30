// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AC_ObjectManager.h"
#include "AC/Object/AC_EnvObject.h"

// Sets default values
AAC_ObjectManager::AAC_ObjectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAC_ObjectManager::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;

	EnvObject = (AAC_EnvObject*)GetWorld()->SpawnActor<AAC_EnvObject>(AAC_EnvObject::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
}

// Called every frame
void AAC_ObjectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AAC_ObjectBase* AAC_ObjectManager::FindObject(const FString& Key)
{
	if (ObjectMap.Find(Key) == nullptr)
		return nullptr;

	return ObjectMap[Key];
}

AAC_ObjectBase* AAC_ObjectManager::FindCharacter(const FString& Key)
{
	return nullptr;
}

void AAC_ObjectManager::SetObjectOnOff(const FString& Key, bool flag)
{

}

void AAC_ObjectManager::AddObject(AAC_ObjectBase* object)
{

}

void AAC_ObjectManager::AddCharacter(AAC_CharacterBase* character)
{
}

TObjectPtr<AAC_EnvObject> AAC_ObjectManager::GetEnvObject()
{
	return EnvObject;
}

