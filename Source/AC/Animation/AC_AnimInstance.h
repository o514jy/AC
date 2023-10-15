// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AC_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class AC_API UAC_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UAC_AnimInstance();

public:
	void LoadMontageObject();

	void PlayAttackMontage(const FString& key);
	void PlaySkillMontage(const FString& key);

	void SetPlayAttackMontage(const FString& key);
	void SetPlaySkillMontage(const FString& key);

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true));
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true));
	UAnimMontage* SKillMontage;

private:
	// Montage

	UAnimMontage* DemonRedAttackMontage;
	UAnimMontage* DemonRedSkillMontage;

	UAnimMontage* DemonDarkAttackMontage;
	UAnimMontage* DemonDarkSkillMontage;

	UAnimMontage* GhoulAbyssAttackMontage;
	UAnimMontage* GhoulAbyssSkillMontage;

	UAnimMontage* GoblinSpearAttackMontage;
	UAnimMontage* GoblinSpearSkillMontage;

	UAnimMontage* GoblinSlingshotAttackMontage;
	UAnimMontage* GoblinSlingshotSkillMontage;

	UAnimMontage* GoblinShamanAttackMontage;
	UAnimMontage* GoblinShamanSkillMontage;

	UAnimMontage* ShroomPoisonAttackMontage;
	UAnimMontage* ShroomPoisonSkillMontage;

private:
	// Notify
	UFUNCTION()
	void AnimNotify_Attack();

};
