// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AC_ObjectManager.h"
#include "AC/Object/AC_EnvObject.h"
#include "AC/Object/AC_PlaceableWaitingSeat1234.h"
#include "AC/Object/AC_PlaceableWaitingSeat5678.h"
#include "AC/Character/AC_CharacterBase.h"
#include "AC/Character/AC_DemonDark.h"
#include "AC/Character/AC_DemonRed.h"
#include "AC/Character/AC_GhoulAbyss.h"
#include "AC/Character/AC_GoblinShaman.h"
#include "AC/Character/AC_GoblinSlingshot.h"
#include "AC/Character/AC_GoblinSpear.h"
#include "AC/Character/AC_ShroomPoison.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AC/Enum/AC_Enum.h"
//temp
#include "Engine/StaticMeshActor.h"

// Sets default values
AAC_ObjectManager::AAC_ObjectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LoadChampionClass();
}

void AAC_ObjectManager::LoadChampionClass()
{
	if (DemonDarkClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<AAC_DemonDark> championAsset(TEXT("Blueprint'/Game/Blueprints/Character/Champion/DemonDark/BP_DemonDark.BP_DemonDark_C'"));
		if (championAsset.Succeeded())
		{
			DemonDarkClass = championAsset.Class;
		}
	}

	if (DemonRedClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<AAC_DemonRed> championAsset(TEXT("Blueprint'/Game/Blueprints/Character/Champion/DemonRed/BP_DemonRed.BP_DemonRed_C'"));
		if (championAsset.Succeeded())
		{
			DemonRedClass = championAsset.Class;
		}
	}

	if (GhoulAbyssClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<AAC_GhoulAbyss> championAsset(TEXT("Blueprint'/Game/Blueprints/Character/Champion/GhoulAbyss/BP_GhoulAbyss.BP_GhoulAbyss_C'"));
		if (championAsset.Succeeded())
		{
			GhoulAbyssClass = championAsset.Class;
		}
	}

	if (GoblinShamanClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<AAC_GoblinShaman> championAsset(TEXT("Blueprint'/Game/Blueprints/Character/Champion/GoblinShaman/BP_GoblinShaman.BP_GoblinShaman_C'"));
		if (championAsset.Succeeded())
		{
			GoblinShamanClass = championAsset.Class;
		}
	}

	if (GoblinSlingshotClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<AAC_GoblinSlingshot> championAsset(TEXT("Blueprint'/Game/Blueprints/Character/Champion/GoblinSlingshot/BP_GoblinSlingshot.BP_GoblinSlingshot_C'"));
		if (championAsset.Succeeded())
		{
			GoblinSlingshotClass = championAsset.Class;
		}
	}

	if (GoblinSpearClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<AAC_GoblinSpear> championAsset(TEXT("Blueprint'/Game/Blueprints/Character/Champion/GoblinSpear/BP_GoblinSpear.BP_GoblinSpear_C'"));
		if (championAsset.Succeeded())
		{
			GoblinSpearClass = championAsset.Class;
		}
	}

	if (ShroomPoisonClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<AAC_ShroomPoison> championAsset(TEXT("Blueprint'/Game/Blueprints/Character/Champion/ShroomPoison/BP_ShroomPoison.BP_ShroomPoison_C'"));
		if (championAsset.Succeeded())
		{
			ShroomPoisonClass = championAsset.Class;
		}
	}

	if (PlaceableObject1x4Class == nullptr)
	{
		static ConstructorHelpers::FClassFinder<AAC_ObjectBase> objectAsset(TEXT("Blueprint'/Game/Blueprints/PlaceableObject/BP_PlaceableObject1x4.BP_PlaceableObject1x4_C'"));
		if (objectAsset.Succeeded())
		{
			PlaceableObject1x4Class = objectAsset.Class;
		}
	}
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

AAC_ObjectBase* AAC_ObjectManager::FindObject(const FString& key)
{
	if (ObjectMap.Contains(key) == false)
		return nullptr;

	return ObjectMap[key];
}

AAC_Champion* AAC_ObjectManager::FindChampion(const FString& key)
{
	if (ChampionMap.Contains(key) == false)
		return nullptr;

	return ChampionMap[key];
}

AAC_Tactician* AAC_ObjectManager::FindTactician(const FString& key)
{
	if (TacticianMap.Contains(key) == false)
		return nullptr;

	return TacticianMap[key];
}

bool AAC_ObjectManager::SetObjectOnOff(const FString& key, bool flag)
{
	if (ObjectMap.Contains(key) == true)
	{
		ObjectMap[key]->SetActorHiddenInGame(!flag);
		return true;
	}

	return false;
}

void AAC_ObjectManager::AddAndSpawnObject(const FString& key, FVector location, FRotator rotation, FActorSpawnParameters spawnParams)
{
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	AAC_ObjectBase* actor = nullptr;

	if (key.Contains(TEXT("PlaceableObject1x4")))
		actor = (AAC_ObjectBase*)GetWorld()->SpawnActor<AAC_ObjectBase>(PlaceableObject1x4Class, location, rotation, spawnParams);

	if (actor != nullptr)
	{
		actor->SetObjectKey(key);
		ObjectMap.Add(key, actor);
	}
}

void AAC_ObjectManager::AddAndSpawnCharacter(const FString& key, FVector location, FRotator rotation, FActorSpawnParameters spawnParams)
{
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	AAC_Champion* actor = nullptr;

	if (key.Contains(TEXT("DemonDark")))
		actor = (AAC_Champion*)GetWorld()->SpawnActor<AAC_DemonDark>(DemonDarkClass, location, rotation, spawnParams);

	if (key.Contains(TEXT("DemonRed")))
		actor = (AAC_Champion*)GetWorld()->SpawnActor<AAC_DemonRed>(DemonRedClass, location, rotation, spawnParams);

	if (key.Contains(TEXT("GhoulAbyss")))
		actor = (AAC_Champion*)GetWorld()->SpawnActor<AAC_GhoulAbyss>(GhoulAbyssClass, location, rotation, spawnParams);

	if (key.Contains(TEXT("GoblinShaman")))
		actor = (AAC_Champion*)GetWorld()->SpawnActor<AAC_GoblinShaman>(GoblinShamanClass, location, rotation, spawnParams);

	if (key.Contains(TEXT("GoblinSlingshot")))
		actor = (AAC_Champion*)GetWorld()->SpawnActor<AAC_GoblinSlingshot>(GoblinSlingshotClass, location, rotation, spawnParams);

	if (key.Contains(TEXT("GoblinSpear")))
		actor = (AAC_Champion*)GetWorld()->SpawnActor<AAC_GoblinSpear>(GoblinSpearClass, location, rotation, spawnParams);

	if (key.Contains(TEXT("ShroomPoison")))
		actor = (AAC_Champion*)GetWorld()->SpawnActor<AAC_ShroomPoison>(ShroomPoisonClass, location, rotation, spawnParams);

	if (actor != nullptr)
	{
		actor->SetObjectKey(key);
		actor->InitChampionStat();
		ChampionMap.Add(key, actor);
	}
}

void AAC_ObjectManager::DestroyChampion(const FString& key)
{
	if (ChampionMap.Contains(key) == false)
		return;

	AAC_Champion* erasedChamp = ChampionMap[key];
	ChampionMap.Remove(key);
	erasedChamp->Destroy();
}

TObjectPtr<AAC_EnvObject> AAC_ObjectManager::GetEnvObject()
{
	return EnvObject;
}

