// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AC_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AC/GameProcessor/AC_GameMaster.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAC_CharacterBase::AAC_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AAC_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAC_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAC_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

TObjectPtr<AAC_GameMaster> AAC_CharacterBase::GetGameMaster()
{
	TArray<AActor*> arrOutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAC_GameMaster::StaticClass(), arrOutActors);

	for (int i = 0; i < arrOutActors.Num(); ++i)
	{
		GameMaster = dynamic_cast<AAC_GameMaster*>(arrOutActors[i]);
	}

	return GameMaster;
}

