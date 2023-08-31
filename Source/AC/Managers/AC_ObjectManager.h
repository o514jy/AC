// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AC_ObjectManager.generated.h"

class AAC_ObjectBase;
class AAC_CharacterBase;
class AAC_Champion;
class AAC_EnvObject;
// Ã¨ÇÇ¾ð
class AAC_DemonDark;
class AAC_DemonRed;
class AAC_GhoulAbyss;
class AAC_GoblinShaman;
class AAC_GoblinSlingshot;
class AAC_GoblinSpear;
class AAC_ShroomPoison;

UCLASS()
class AC_API AAC_ObjectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_ObjectManager();

	void LoadChampionClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	AAC_ObjectBase* FindObject(const FString& Key);
	AAC_ObjectBase* FindCharacter(const FString& Key);

	void SetObjectOnOff(const FString& Key, bool flag);

	void AddObject(AAC_ObjectBase* object);
	void AddAndSpawnCharacter(const FString& key, FVector location = FVector(0, 0, 0), FRotator rotation = FRotator(0, 0, 0), FActorSpawnParameters spawnParams = FActorSpawnParameters());

	TObjectPtr<AAC_EnvObject> GetEnvObject();

private:
	TMap<FString/*key*/, AAC_ObjectBase*> ObjectMap;
	TMap<FString/*key*/, AAC_Champion*> ChampionMap;
	
	TObjectPtr<AAC_EnvObject> EnvObject;

	UPROPERTY()
	TSubclassOf<AAC_DemonDark> DemonDarkClass;

	UPROPERTY()
	TSubclassOf<AAC_DemonRed> DemonRedClass;

	UPROPERTY()
	TSubclassOf<AAC_GhoulAbyss> GhoulAbyssClass;

	UPROPERTY()
	TSubclassOf<AAC_GoblinShaman> GoblinShamanClass;

	UPROPERTY()
	TSubclassOf<AAC_GoblinSlingshot> GoblinSlingshotClass;

	UPROPERTY()
	TSubclassOf<AAC_GoblinSpear> GoblinSpearClass;

	UPROPERTY()
	TSubclassOf<AAC_ShroomPoison> ShroomPoisonClass;
};
