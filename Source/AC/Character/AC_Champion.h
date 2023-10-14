// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AC_CharacterBase.h"
#include "../Enum/AC_Enum.h"
#include "../Interface/AC_TargetInterface.h"
#include "AC_Champion.generated.h"

//class UPawnSensingComponent;
class AAC_Tactician;
class UAC_AnimInstance;
class USphereComponent;

USTRUCT(BlueprintType)
struct FChampionStat
{
	GENERATED_BODY()

public:
	// Combat Stat
	UPROPERTY()
	int Health;

	UPROPERTY()
	int MaxHealth;

	UPROPERTY()
	int Mana;

	UPROPERTY()
	int MaxMana;

	UPROPERTY()
	int AttackRange;

	UPROPERTY()
	int AttackDamage;

	UPROPERTY()
	float AttackSpeedRatio;

	UPROPERTY()
	int AbilityPower;

	UPROPERTY()
	float MoveSpeedRatio;

	// Info Stat
	UPROPERTY()
	int ChampionCost;

	UPROPERTY()
	int ChampionStarLevel;

	UPROPERTY()
	FString Ethnic;

	UPROPERTY()
	FString Occupation;
};

UCLASS()
class AC_API AAC_Champion : public AAC_CharacterBase, public IAC_TargetInterface
{
	GENERATED_BODY()
	
public:
	AAC_Champion();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void TickIdle();
	virtual void TickMove();
	virtual void TickAttack();
	virtual void TickSkill();
	virtual void TickDead();

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

public:
	// Pawn Sensing 사용
	//UPROPERTY(VisibleAnywhere)
	//UPawnSensingComponent* PawnSensing;

	//UFUNCTION()
	//void OnPawnSeen(APawn* seenPawn);

public:
	// 탐지 관련
	// 탐지 범위 설정
	UPROPERTY(VisibleAnywhere)
	USphereComponent* DetectSphereCollision;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* AttackSphereCollision;

	void BindOverlapDelegate(bool flag);

	UFUNCTION()
	void OnDetectSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnAttackSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDetectSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void OnAttackSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsInAttackRange(AAC_Champion* inTarget);

	bool GetbInArena() { return bInArena; }
	void SetbInArena(bool flag) { bInArena = flag; }

private:
	TArray<TObjectPtr<AAC_Champion>> DetectedChampionArr;

	bool bInArena = false;

public:

	void SetState(EState newState);
	EState GetState() { return State; }

	void SetCombatTarget(AAC_Champion* inTarget);
	TObjectPtr<AAC_Champion> GetCombatTarget();

	void SetOwnerTactician(TObjectPtr<AAC_Tactician> ownerTactician);
	TObjectPtr<AAC_Tactician> GetOwnerTactician();

	void SetbIsEnemy(bool isEnemy) { bIsEnemy = isEnemy; }
	bool GetbIsEnemy() { return bIsEnemy; }

public:
	// Animation
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool bIsAttacking = false;
	bool bIsSkilling = false;

public:
	virtual void InitChampionStat();

	FChampionStat GetChampionStat() { return ChampionStat; }

	// process state
	void MoveToCombatTarget(AAC_Champion* inTarget);

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	FChampionStat ChampionStat;

	EState State = EState::Idle;

	// Timer
	FTimerHandle WaitTimer;

	// Combat Target
	TObjectPtr<AAC_Champion> CombatTarget = nullptr;

	// Owner
	TObjectPtr<AAC_Tactician> OwnerTactician = nullptr;

	// creep인지 확인
	bool bIsEnemy = false;
		
	UPROPERTY()
	UAC_AnimInstance* ABAnim;

};
