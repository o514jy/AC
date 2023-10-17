// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AC_Champion.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "AC/Data/AC_ChampionInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "AC/Character/AC_Tactician.h"
#include "AC/Animation/AC_AnimInstance.h"
// Manager
#include "AC/Managers/AC_DataManager.h"
#include "AC/Managers/AC_ObjectManager.h"
#include "AC/Library/AC_FunctionLibrary.h"
#include "AC/Enum/AC_Enum.h"

#include "AC/GameProcessor/AC_GameMaster.h"
#include "AC/Enum/AC_Enum.h"
#include "AC/Object/AC_EnvObject.h"
#include "Engine/StaticMeshActor.h"
#include "AC/UI/AC_HealthBarUI.h"
#include "Components/WidgetComponent.h"

AAC_Champion::AAC_Champion()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Champion"));

	GetCharacterMovement()->SetMovementMode(MOVE_Falling);

	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	// AI 설정
	//static ConstructorHelpers::FClassFinder<AController> AIController(TEXT("Blueprint'/Game/Blueprints/Controller/BP_AIController.BP_AIController_C'"));
	//if (AIController.Succeeded())
	//{
	//	AIControllerClass = AIController.Class;
	//}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DetectSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphereCollision"));
	DetectSphereCollision->SetupAttachment(RootComponent);
	DetectSphereCollision->InitSphereRadius(500.0f);
	DetectSphereCollision->SetCollisionProfileName(TEXT("DetectChampion"));

	AttackSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphereCollision"));
	AttackSphereCollision->SetupAttachment(RootComponent);
	AttackSphereCollision->InitSphereRadius(100.0f);
	AttackSphereCollision->SetCollisionProfileName(TEXT("AttackChampion"));

	if (HealthBarUIClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<UAC_HealthBarUI> StoreUI(TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_HealthBar.WBP_HealthBar_C'"));
		if (StoreUI.Succeeded())
			HealthBarUIClass = StoreUI.Class;
	}

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(RootComponent);
	HealthBar->SetDrawAtDesiredSize(true);
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBar->SetWidgetClass(HealthBarUIClass);
	
	FTransform initTransform = HealthBar->GetRelativeTransform();
	initTransform.SetLocation(FVector(0, 0, 80.f));
	HealthBar->SetRelativeTransform(initTransform);

	//PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	//PawnSensing->SetPeripheralVisionAngle(80.f);
	//PawnSensing->bOnlySensePlayers = false;

	// Set an initial velocity to make the Character fall
	//FVector initialVelocity = FVector(0.0f, 0.0f, -1000.0f);
	//GetCharacterMovement()->Velocity = initialVelocity;
	//GetCharacterMovement()->GravityScale = 1.0f;

	//GetCapsuleComponent()->SetSimulatePhysics(true);
}

void AAC_Champion::BeginPlay()
{
	Super::BeginPlay();

	this->SpawnDefaultController();

	ABAnim = Cast<UAC_AnimInstance>(GetMesh()->GetAnimInstance());

	if (ABAnim)
		ABAnim->OnMontageEnded.AddDynamic(this, &AAC_Champion::OnMontageEnded);

	//if (PawnSensing)
	//	PawnSensing->OnSeePawn.AddDynamic(this, &AAC_Champion::OnPawnSeen);

	//BindOverlapDelegate();
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
	// Ready 상태일 경우 creep이 스폰되기 전에 먼저 이벤트 bind
	//if (GetGameMaster()->GetRoundState() == EGameState::Ready)
		//BindOverlapDelegate(true);

	// 대기석에 있는 경우 계속 Idle 상태
	if (this->GetbInArena() == false)
		return;

	// Battle 상태일 경우 움직이기 시작
	if (GetGameMaster()->GetRoundState() == EGameState::Battle)
	{
		SetState(EState::Move);
		return;
	}

	// Ready 상태일때 위치 저장, 이미 저장되있으면 X, 크립이면 위치조정이 필요없기때문에 X
	if (ReadyLocation != FVector(0, 0, 0) || this->GetbIsEnemy() == true)
		return;

	if (GetGameMaster()->GetRoundState() == EGameState::Ready)
		ReadyLocation = GetActorLocation();

}

void AAC_Champion::TickMove()
{
	// 공격 사거리까지 타겟을 향해 움직이고 닿았으면 attack으로 전환, 마나가 풀일 경우 skill로 전환
	
	// Prepare 상태일 경우 Idle로
	if (GetGameMaster()->GetRoundState() == EGameState::Prepare)
	{
		SetState(EState::Idle);
		return;
	}

	// 가장 가까운 타겟을 고른다.
	AAC_Champion* closestTarget = nullptr;
	float closestDistance = MAX_FLT;
	
	for (AAC_Champion* champion : DetectedChampionArr)
	{
		if (champion)
		{
			FVector championLocation = champion->GetActorLocation();
			float Distance = FVector::Dist(this->GetActorLocation(), championLocation);

			if (Distance < closestDistance)
			{
				closestTarget = champion;
				closestDistance = Distance;
			}
		}
	}

	CombatTarget = closestTarget;

	// 타겟을 향해 움직인다
	MoveToCombatTarget(CombatTarget);

	// 공격 사거리에 닿았으면 attack으로 전환
	if (IsInAttackRange(CombatTarget))
	{
		// 마나가 덜 찼을 경우 attack, 다 찼을 경우 skill
		auto aiController = Cast<AAIController>(GetController());
		aiController->StopMovement();
		SetState(EState::Attack);
	}
	else
	{
		// 안닿았으면 다시 move로
		SetState(EState::Move);
	}
}

void AAC_Champion::TickAttack()
{
	// prepare상태가 되면 전투X
	if (GetGameMaster()->GetRoundState() == EGameState::Prepare)
	{
		CombatTarget = nullptr;
		SetState(EState::Move);
		return;
	}

	// 이미 공격중일 경우 skip
	if (bIsAttacking == true)
		return;

	// 상대가 죽었을 경우 skip
	if (CombatTarget->GetState() == EState::Dead)
	{
		CombatTarget = nullptr;
		SetState(EState::Move);
		return;
	}

	// 공격속도에 따라 타겟이 사거리 내에 있을 경우 attack, 없을경우 move로 전환
	if (IsInAttackRange(CombatTarget))
	{
		bIsAttacking = true;
		ABAnim->PlayAttackMontage(GetObjectKey());
	}
	else
	{
		SetState(EState::Move);
	}
}

void AAC_Champion::TickSkill()
{
	// skill 사용 이후 타겟이 사거리 내에 있을 경우 attack, 없을 경우 move로 전환
}

void AAC_Champion::TickDead()
{
	// actor 삭제? 전투페이즈가 끝나면..dead가 아니라 invisible로 해야되나
	// prepare상태가 되면 전투X
	if (GetGameMaster()->GetRoundState() == EGameState::Prepare)
	{
		SetState(EState::Idle);
		return;
	}
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

void AAC_Champion::SendDamage()
{
	int finalDamage = this->ChampionStat.AttackDamage + this->ChampionStat.AbilityPower;

	if (CombatTarget == nullptr)
		return;

	CombatTarget->OnDamaged(finalDamage);
}

bool AAC_Champion::OnDamaged(int damage)
{
	if (CombatTarget == nullptr)
		return false;

	this->ChampionStat.Health = fmax(0, this->ChampionStat.Health - damage);

	UAC_HealthBarUI* widget = Cast<UAC_HealthBarUI>(HealthBar->GetUserWidgetObject());
	widget->SetHealthBar((float)this->ChampionStat.Health / this->ChampionStat.MaxHealth);

	if (this->ChampionStat.Health == 0)
		SetState(EState::Dead);

	return true;
}

//void AAC_Champion::OnPawnSeen(APawn* seenPawn)
//{
//	if (seenPawn == nullptr)
//		return;
//
//	// champion에만 반응
//	CombatTarget = Cast<AAC_Champion>(seenPawn);
//
//	if (CombatTarget == nullptr)
//		return;
//
//	// 서로 아군일 경우 패스
//	if (this->GetbIsEnemy() == true)
//	{
//		if (CombatTarget->GetbIsEnemy() == true)
//		{
//			CombatTarget = nullptr;
//			return;
//		}
//	}
//	else
//	{
//		if (CombatTarget->GetbIsEnemy() == false)
//		{
//			CombatTarget = nullptr;
//			return;
//		}
//	}
//
//	SetState(EState::Move);
//}

void AAC_Champion::BindOverlapDelegate(bool flag)
{

	auto a = this;

	if (flag == true)
	{
		if (DetectSphereCollision->OnComponentBeginOverlap.IsBound() == true)
			return;

		if (DetectSphereCollision->OnComponentEndOverlap.IsBound() == true)
			return;

		if (AttackSphereCollision->OnComponentBeginOverlap.IsBound() == true)
			return;

		if (AttackSphereCollision->OnComponentEndOverlap.IsBound() == true)
			return;

		DetectSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AAC_Champion::OnDetectSphereOverlapBegin);
		DetectSphereCollision->OnComponentEndOverlap.AddDynamic(this, &AAC_Champion::OnDetectSphereOverlapEnd);

		AttackSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AAC_Champion::OnAttackSphereOverlapBegin);
		AttackSphereCollision->OnComponentEndOverlap.AddDynamic(this, &AAC_Champion::OnAttackSphereOverlapEnd);
	}
	else
	{
		DetectSphereCollision->OnComponentBeginOverlap.Clear();
		DetectSphereCollision->OnComponentEndOverlap.Clear();

		AttackSphereCollision->OnComponentBeginOverlap.Clear();
		AttackSphereCollision->OnComponentEndOverlap.Clear();
	}

}

void AAC_Champion::OnDetectSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this)
		return;

	AAC_Champion* inTarget = Cast<AAC_Champion>(OtherActor);

	// arena 안에 없는 경우 고려X
	if (inTarget->GetbInArena() == false)
		return;
	
	// 서로 팀인 경우 고려X
	if (this->GetbIsEnemy() == true && inTarget->GetbIsEnemy() == true)
		return;

	if (this->GetbIsEnemy() == false && inTarget->GetbIsEnemy() == false)
		return;

	DetectedChampionArr.Add(inTarget);
}

void AAC_Champion::OnAttackSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this)
		return;

	AAC_Champion* inTarget = Cast<AAC_Champion>(OtherActor);

	if (inTarget == nullptr)
		return;

	// arena 안에 없는 경우 고려X
	if (inTarget->GetbInArena() == false)
		return;

	// 서로 팀인 경우 고려X
	if (this->GetbIsEnemy() == true && inTarget->GetbIsEnemy() == true)
		return;

	if (this->GetbIsEnemy() == false && inTarget->GetbIsEnemy() == false)
		return;

	// 이미 탐색목록에 있으면 고려X
	if (DetectedChampionArr.Find(inTarget) != INDEX_NONE)
		return;

	DetectedChampionArr.Add(inTarget);
}

void AAC_Champion::OnDetectSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr || OtherActor == this)
		return;

	AAC_Champion* inTarget = Cast<AAC_Champion>(OtherActor);

	int index;
	if (DetectedChampionArr.Find(inTarget, index) == true)
		DetectedChampionArr.RemoveAt(index);
}

void AAC_Champion::OnAttackSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*if (OtherActor == nullptr || OtherActor == this)
		return;

	AAC_Champion* inTarget = Cast<AAC_Champion>(OtherActor);

	int index;
	if (DetectedChampionArr.Find(inTarget, index) == true)
		DetectedChampionArr.RemoveAt(index);*/
}

void AAC_Champion::SetState(EState newState)
{
	switch (newState)
	{
	case EState::Idle:
		State = EState::Idle;
		if (GetGameMaster()->GetRoundState() == EGameState::Prepare)
		{
			DetectedChampionArr.Empty();

			this->SetActorLocation(ReadyLocation);

			auto aiController = Cast<AAIController>(GetController());
			aiController->StopMovement();

			this->SetActorHiddenInGame(false);
			this->SetActorEnableCollision(true);

			this->ChampionStat.Health = this->ChampionStat.MaxHealth;

			UAC_HealthBarUI* widget = Cast<UAC_HealthBarUI>(HealthBar->GetUserWidgetObject());
			widget->SetHealthBar((float)this->ChampionStat.Health / this->ChampionStat.MaxHealth);
		}
		BindOverlapDelegate(false);
		break;
	case EState::Move:
		State = EState::Move;
		BindOverlapDelegate(true);
		break;
	case EState::Attack:
		State = EState::Attack;
		BindOverlapDelegate(true);
		break;
	case EState::Skill:
		State = EState::Skill;
		BindOverlapDelegate(true);
		break;
	case EState::Dead:
		State = EState::Dead;
		this->SetActorHiddenInGame(true);
		this->SetActorEnableCollision(false);
		BindOverlapDelegate(false);
		break;
	}
}

bool AAC_Champion::IsInAttackRange(AAC_Champion* inTarget)
{
	if (inTarget == nullptr)
		return false;

	double distanceToTarget = (inTarget->GetActorLocation() - GetActorLocation()).Size();

	if (distanceToTarget <= GetChampionStat().AttackRange)
		return true;

	return false;
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

void AAC_Champion::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
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
	ChampionStat.MaxHealth = initData.MaxHealth;
	ChampionStat.Mana = initData.Mana;
	ChampionStat.MaxMana = initData.MaxMana;
	ChampionStat.AttackRange = initData.AttackRange;
	ChampionStat.AttackDamage = initData.AttackDamage;
	ChampionStat.AttackSpeedRatio = initData.AttackSpeedRatio;
	ChampionStat.AbilityPower = initData.AbilityPower;
	ChampionStat.MoveSpeedRatio = initData.MoveSpeedRatio;
	ChampionStat.ChampionCost = initData.ChampionCost;
	ChampionStat.ChampionStarLevel = initData.ChampionStarLevel;
	ChampionStat.Ethnic = initData.Ethnic;
	ChampionStat.Occupation = initData.Occupation;

	UAC_HealthBarUI* widget = Cast<UAC_HealthBarUI>(HealthBar->GetUserWidgetObject());
	widget->SetHealthBar((float)this->ChampionStat.Health / this->ChampionStat.MaxHealth);
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
		//if (navPath.IsValid())
		//{
		//	TArray<FNavPathPoint>& pathPoints = navPath->GetPathPoints();
		//	for (const auto& Point : pathPoints)
		//	{
		//		const FVector& location = Point.Location;
		//		DrawDebugSphere(GetWorld(), location, 12.f, 12, FColor::Green, false, 10.f);
		//	}
		//}
	}

	if (inTarget != nullptr)
		return;

	// 타겟이 없을 때 살짝 앞으로 감
	
	// 현재 위치
	//FVector CurrentLocation = GetActorLocation();

	// 바라보고 있는 방향
	//FRotator CurrentRotation = GetControlRotation();

	// 바라보고 있는 방향으로 이동할 거리
	//float MoveDistance = 100.0f; // 이동 거리

	// 바라보고 있는 방향으로 이동할 위치
	//FVector DestinationLocation = CurrentLocation + CurrentRotation.Vector() * MoveDistance;

	// 상대쪽 진형 위치로
	FVector DestinationLocation = UAC_FunctionLibrary::GetObjectManager(GetWorld())->GetEnvObject()->GetArena()[Def_ArenaRightUp]->GetActorLocation();
	if (this->GetbIsEnemy() == false)
		DestinationLocation.X += 300.f;
	else
		DestinationLocation.X -= 300.f;

	// SimpleMoveToLocation 목적지로 이동
	if (aiController)
	{
		aiController->MoveToLocation(DestinationLocation, -1.0f, true, true, true);
	}
}
