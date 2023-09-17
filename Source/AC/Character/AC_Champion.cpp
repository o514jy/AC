// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AC_Champion.h"
#include "Components/CapsuleComponent.h"
#include "AC/Data/AC_ChampionInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
// Manager
#include "AC/Managers/AC_DataManager.h"
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Enum/AC_Enum.h"

AAC_Champion::AAC_Champion()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Champion"));

	GetCharacterMovement()->SetMovementMode(MOVE_Falling);

	// AI 설정
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
	// 비전투상태이거나 적이 모두 죽었을 경우 대기
}

void AAC_Champion::TickMove()
{
	// 공격 사거리까지 타겟을 향해 움직이고 닿았으면 attack으로 전환, 마나가 풀일 경우 skill로 전환
	MoveToCombatTarget(CombatTarget);
}

void AAC_Champion::TickAttack()
{
	// 공격속도에 따라 타겟이 사거리 내에 있을 경우 attack, 없을경우 move로 전환
}

void AAC_Champion::TickSkill()
{
	// skill 사용 이후 타겟이 사거리 내에 있을 경우 attack, 없을 경우 move로 전환
}

void AAC_Champion::TickDead()
{
	// actor 삭제? 전투페이즈가 끝나면..dead가 아니라 invisible로 해야되나
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
	State = EState::Move;
}

void AAC_Champion::SetState(EState newState)
{

}

void AAC_Champion::SetCombatTarget(AAC_Champion* inTarget)
{
	CombatTarget = inTarget;
}

TObjectPtr<AAC_Champion> AAC_Champion::GetCombatTarget()
{
	return TObjectPtr<AAC_Champion>();
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
		moveRequest.SetAcceptanceRadius(15.f);

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
