// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AC_Champion.h"
#include "Components/CapsuleComponent.h"
#include "AC/Data/AC_ChampionInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "AC/Character/AC_Tactician.h"
// Manager
#include "AC/Managers/AC_DataManager.h"
#include "AC/Managers/AC_ObjectManager.h"
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Enum/AC_Enum.h"

#include "AC/GameProcessor/AC_GameMaster.h"
#include "AC/Enum/AC_Enum.h"

AAC_Champion::AAC_Champion()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Champion"));

	GetCharacterMovement()->SetMovementMode(MOVE_Falling);

	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	// AI ����
	//static ConstructorHelpers::FClassFinder<AController> AIController(TEXT("Blueprint'/Game/Blueprints/Controller/BP_AIController.BP_AIController_C'"));
	//if (AIController.Succeeded())
	//{
	//	AIControllerClass = AIController.Class;
	//}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SetPeripheralVisionAngle(80.f);
	PawnSensing->bOnlySensePlayers = false;

	// Set an initial velocity to make the Character fall
	//FVector initialVelocity = FVector(0.0f, 0.0f, -1000.0f);
	//GetCharacterMovement()->Velocity = initialVelocity;
	//GetCharacterMovement()->GravityScale = 1.0f;

	//GetCapsuleComponent()->SetSimulatePhysics(true);
}

void AAC_Champion::BeginPlay()
{
	Super::BeginPlay();

	InitChampionStat();

	this->SpawnDefaultController();

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
		break;
	case EState::Move:
		TickMove();
		break;
	case EState::Attack:
		TickAttack();
		break;
	case EState::Skill:
		TickSkill();
		break;
	case EState::Dead:
		TickDead();
		break;
	}
}

void AAC_Champion::TickIdle()
{
	// Battle ������ ��� �����̱� ����
	if (GetGameMaster()->GetRoundState() == EGameState::Battle)
	{
		SetState(EState::Move);
	}
}

void AAC_Champion::TickMove()
{
	// ���� ��Ÿ����� Ÿ���� ���� �����̰� ������� attack���� ��ȯ, ������ Ǯ�� ��� skill�� ��ȯ
	MoveToCombatTarget(CombatTarget);

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
	if (seenPawn == nullptr)
		return;

	// champion���� ����
	CombatTarget = Cast<AAC_Champion>(seenPawn);

	if (CombatTarget == nullptr)
		return;

	// ���� �Ʊ��� ��� �н�
	if (this->GetbIsEnemy() == true)
	{
		if (CombatTarget->GetbIsEnemy() == true)
		{
			CombatTarget = nullptr;
			return;
		}
	}
	else
	{
		if (CombatTarget->GetbIsEnemy() == false)
		{
			CombatTarget = nullptr;
			return;
		}
	}

	State = EState::Move;
}

void AAC_Champion::SetState(EState newState)
{
	switch (newState)
	{
	case EState::Idle:
		State = EState::Idle;
		break;
	case EState::Move:
		State = EState::Move;
		break;
	case EState::Attack:
		State = EState::Attack;
		break;
	case EState::Skill:
		State = EState::Skill;
		break;
	case EState::Dead:
		State = EState::Dead;
		break;
	}
}

void AAC_Champion::SetCombatTarget(AAC_Champion* inTarget)
{
	CombatTarget = inTarget;
}

TObjectPtr<AAC_Champion> AAC_Champion::GetCombatTarget()
{
	return CombatTarget;
}

void AAC_Champion::SetOwnerTactician(TObjectPtr<AAC_Tactician> ownerTactician)
{
	OwnerTactician = ownerTactician;
}

TObjectPtr<AAC_Tactician> AAC_Champion::GetOwnerTactician()
{
	return OwnerTactician;
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

void AAC_Champion::MoveToCombatTarget(AAC_Champion* inTarget)
{
	auto aiController = Cast<AAIController>(GetController());
	if (aiController && inTarget)
	{
		FAIMoveRequest moveRequest;
		moveRequest.SetGoalActor(inTarget);
		moveRequest.SetAcceptanceRadius(30.f);

		FNavPathSharedPtr navPath;
		aiController->MoveTo(moveRequest, &navPath);

		// Debug
		if (navPath.IsValid())
		{
			TArray<FNavPathPoint>& pathPoints = navPath->GetPathPoints();
			for (const auto& Point : pathPoints)
			{
				const FVector& location = Point.Location;
				DrawDebugSphere(GetWorld(), location, 12.f, 12, FColor::Green, false, 10.f);
			}
		}
	}
}
