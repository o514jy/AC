// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AC_ObjectManager.generated.h"

class AAC_ObjectBase;
class AAC_CharacterBase;
class AAC_EnvObject;

UCLASS()
class AC_API AAC_ObjectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAC_ObjectManager();

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
	void AddCharacter(AAC_CharacterBase* character);

	TObjectPtr<AAC_EnvObject> GetEnvObject();

private:
	TMap<FString/*key*/, AAC_ObjectBase*> ObjectMap;
	TMap<FString/*key*/, AAC_CharacterBase*> CharacterMap;
	
	TObjectPtr<AAC_EnvObject> EnvObject;
};
