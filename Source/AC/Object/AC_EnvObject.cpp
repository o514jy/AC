// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/AC_EnvObject.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "AC/Enum/AC_Enum.h"
#include "NiagaraActor.h"

// Sets default values
AAC_EnvObject::AAC_EnvObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAC_EnvObject::BeginPlay()
{
	Super::BeginPlay();

	// �ʿ� ��ġ�� actor ��ĵ
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OUT arrOutActors);

	GetMainCamera();

	GetWaitingSeat();

	GetArena();

	GetItemSeat();

	GetFire();
}

// Called every frame
void AAC_EnvObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<TObjectPtr<AActor>> AAC_EnvObject::GetarrOutActors()
{
	check(!arrOutActors.IsEmpty());

	return arrOutActors;
}

TObjectPtr<AActor> AAC_EnvObject::GetMainCamera()
{
	check(!arrOutActors.IsEmpty());

	if (MainCamera == nullptr)
	{
		for (int i = 0; i < arrOutActors.Num(); i++)
		{
			if (arrOutActors[i]->GetActorLabel() == TEXT("MainCamera"))
				MainCamera = arrOutActors[i];
		}
	}


	return MainCamera;
}

TArray<TObjectPtr<AStaticMeshActor>> AAC_EnvObject::GetWaitingSeat()
{
	check(!arrOutActors.IsEmpty());

	
	if (SM_WaitingSeat.IsEmpty() == true)
	{
		SM_WaitingSeat.Init(nullptr, 2);
		for (int i = 0; i < arrOutActors.Num(); i++)
		{
			if (arrOutActors[i]->GetActorLabel() == TEXT("SM_WaitingSeat1234"))
			{
				AStaticMeshActor* SM_WaitingSeat1234 = Cast<AStaticMeshActor>(arrOutActors[i]);
				SM_WaitingSeat[Def_WaitingSeat1234] = SM_WaitingSeat1234;
			}

			if (arrOutActors[i]->GetActorLabel() == TEXT("SM_WaitingSeat5678"))
			{
				AStaticMeshActor* SM_WaitingSeat5678 = Cast<AStaticMeshActor>(arrOutActors[i]);
				SM_WaitingSeat[Def_WaitingSeat5678] = SM_WaitingSeat5678;
			}
		}
	}

	return SM_WaitingSeat;
}

TArray<TObjectPtr<AStaticMeshActor>> AAC_EnvObject::GetArena()
{
	check(!arrOutActors.IsEmpty());

	if (SM_Arena.IsEmpty() == true)
	{
		SM_Arena.Init(nullptr, 4);
		for (int i = 0; i < arrOutActors.Num(); i++)
		{
			if (arrOutActors[i]->GetActorLabel() == TEXT("SM_Arena_LeftDown"))
			{
				AStaticMeshActor* SM_Arena_LeftDown = Cast<AStaticMeshActor>(arrOutActors[i]);
				SM_Arena[Def_ArenaLeftDown] = SM_Arena_LeftDown;
			}

			if (arrOutActors[i]->GetActorLabel() == TEXT("SM_Arena_LeftUp"))
			{
				AStaticMeshActor* SM_Arena_LeftUp = Cast<AStaticMeshActor>(arrOutActors[i]);
				SM_Arena[Def_ArenaLeftUp] = SM_Arena_LeftUp;
			}
			
			if (arrOutActors[i]->GetActorLabel() == TEXT("SM_Arena_RightDown"))
			{
				AStaticMeshActor* SM_Arena_RightDown = Cast<AStaticMeshActor>(arrOutActors[i]);
				SM_Arena[Def_ArenaRightDown] = SM_Arena_RightDown;
			}

			if (arrOutActors[i]->GetActorLabel() == TEXT("SM_Arena_RightUp"))
			{
				AStaticMeshActor* SM_Arena_RightUp = Cast<AStaticMeshActor>(arrOutActors[i]);
				SM_Arena[Def_ArenaRightUp] = SM_Arena_RightUp;
			}
		}
	}

	return SM_Arena;
}

TObjectPtr<AStaticMeshActor> AAC_EnvObject::GetItemSeat()
{
	check(!arrOutActors.IsEmpty());

	if (SM_ItemSeat == nullptr)
	{
		for (int i = 0; i < arrOutActors.Num(); i++)
		{
			if (arrOutActors[i]->GetActorLabel() == TEXT("SM_ItemSeat"))
				SM_ItemSeat = Cast<AStaticMeshActor>(arrOutActors[i]);
		}
	}

	return SM_ItemSeat;
}

TArray<TObjectPtr<ANiagaraActor>> AAC_EnvObject::GetFire()
{
	if (NS_Fire.IsEmpty() == true)
	{
		NS_Fire.Init(nullptr, 5);
		for (int i = 0; i < arrOutActors.Num(); i++)
		{
			if (arrOutActors[i]->GetActorLabel() == TEXT("NS_Fire1"))
			{
				ANiagaraActor* fire1 = Cast<ANiagaraActor>(arrOutActors[i]);
				NS_Fire[Def_NS_Fire1] = fire1;
			}

			if (arrOutActors[i]->GetActorLabel() == TEXT("NS_Fire2"))
			{
				ANiagaraActor* fire2 = Cast<ANiagaraActor>(arrOutActors[i]);
				NS_Fire[Def_NS_Fire2] = fire2;
			}

			if (arrOutActors[i]->GetActorLabel() == TEXT("NS_Fire3"))
			{
				ANiagaraActor* fire3 = Cast<ANiagaraActor>(arrOutActors[i]);
				NS_Fire[Def_NS_Fire3] = fire3;
			}

			if (arrOutActors[i]->GetActorLabel() == TEXT("NS_Fire4"))
			{
				ANiagaraActor* fire4 = Cast<ANiagaraActor>(arrOutActors[i]);
				NS_Fire[Def_NS_Fire4] = fire4;
			}

			if (arrOutActors[i]->GetActorLabel() == TEXT("NS_Fire5"))
			{
				ANiagaraActor* fire5 = Cast<ANiagaraActor>(arrOutActors[i]);
				NS_Fire[Def_NS_Fire5] = fire5;
			}
		}
	}

	return NS_Fire;
}



