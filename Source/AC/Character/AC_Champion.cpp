// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AC_Champion.h"
#include "Components/CapsuleComponent.h"
#include "AC/Data/AC_ChampionInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
// Manager
#include "AC/Managers/AC_DataManager.h"
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Enum/AC_Enum.h"

AAC_Champion::AAC_Champion()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Champion"));

	GetCharacterMovement()->SetMovementMode(MOVE_Falling);

	// AI ����
	static ConstructorHelpers::FClassFinder<AController> AIController(TEXT("Blueprint'/Game/Blueprints/Controller/BP_AIController.BP_AIController_C'"));
	if (AIController.Succeeded())
	{
		AIControllerClass = AIController.Class;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SetPeripheralVisionAngle(90.f);

	// Set an initial velocity to make the Character fall
	//FVector initialVelocity = FVector(0.0f, 0.0f, -1000.0f);
	//GetCharacterMovement()->Velocity = initialVelocity;
	//GetCharacterMovement()->GravityScale = 1.0f;

	//GetCapsuleComponent()->SetSimulatePhysics(true);
}

void AAC_Champion::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensing)
		PawnSensing->OnSeePawn.AddDynamic(this, &AAC_Champion::OnPawnSeen);
}

void AAC_Champion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
	case EState::Idle:
		TickIdle();
	case EState::Move:
		TickMove();
	case EState::Attack:
		TickAttack();
	case EState::Skill:
		TickSkill();
	case EState::Dead:
		TickDead();
	}
}

void AAC_Champion::TickIdle()
{
	// �����������̰ų� ���� ��� �׾��� ��� ���
}

void AAC_Champion::TickMove()
{
	// ���� ��Ÿ����� Ÿ���� ���� �����̰� ������� attack���� ��ȯ, ������ Ǯ�� ��� skill�� ��ȯ
	
}

void AAC_Champion::TickAttack()
{
	// ���ݼӵ��� ���� Ÿ���� ��Ÿ� ���� ���� ��� attack, ������� move�� ��ȯ
}

void AAC_Champion::TickSkill()
{
	// skill ��� ���� Ÿ���� ��Ÿ� ���� ���� ��� attack, ���� ��� move�� ��ȯ
}

void AAC_Champion::TickDead()
{
	// actor ����? ��������� ������..dead�� �ƴ϶� invisible�� �ؾߵǳ�
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

void AAC_Champion::OnPawnSeen(APawn* seenPawn)
{

}

void AAC_Champion::SetState(EState newState)
{

}

EState AAC_Champion::GetState()
{
	return State;
}

void AAC_Champion::InitChampionStat()
{
	UAC_ChampionInfo* info = UAC_FunctionLibrary::GetDataManager(GetWorld())->GetChampionInfo();
	FChampionInfo initData;
	for (auto& index : info->ChampionInfoArr)
	{
		if (Key.Contains(index.Key))
			initData = index;
	}

	ChampionStat.Health = initData.Health;
	ChampionStat.AttackRange = initData.AttackRange;
	ChampionStat.AttackDamage = initData.AttackDamage;
	ChampionStat.AttackSpeedRatio = initData.AttackSpeedRatio;
	ChampionStat.AbilityPower = initData.AbilityPower;
	ChampionStat.MoveSpeedRatio = initData.MoveSpeedRatio;
	ChampionStat.ChampionCost = initData.ChampionCost;
	ChampionStat.ChampionStarLevel = initData.ChampionStarLevel;
	ChampionStat.Ethnic = initData.Ethnic;
	ChampionStat.Occupation = initData.Occupation;
}
