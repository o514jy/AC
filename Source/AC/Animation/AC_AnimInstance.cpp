// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AC_AnimInstance.h"

UAC_AnimInstance::UAC_AnimInstance()
{
	LoadMontageObject();
}

void UAC_AnimInstance::LoadMontageObject()
{
	// demon red
	static ConstructorHelpers::FObjectFinder <UAnimMontage> DEMON_RED_ATTACK_MONTAGE
	(TEXT("/Game/Assets/Enemies/Demon/Animations/AM_Demon_Attack_L_Montage.AM_Demon_Attack_L_Montage"));
	if (DEMON_RED_ATTACK_MONTAGE.Succeeded())
		DemonRedAttackMontage = DEMON_RED_ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> DEMON_RED_SKILL_MONTAGE
	(TEXT("/Game/Assets/Enemies/Demon/Animations/AM_Demon_Red_Tailspin_Montage.AM_Demon_Red_Tailspin_Montage"));
	if (DEMON_RED_SKILL_MONTAGE.Succeeded())
		DemonRedSkillMontage = DEMON_RED_SKILL_MONTAGE.Object;

	// demon dark
	static ConstructorHelpers::FObjectFinder <UAnimMontage> DEMON_DARK_ATTACK_MONTAGE
	(TEXT("/Game/Assets/Enemies/Demon/Animations/AM_Demon_Attack_R_Montage.AM_Demon_Attack_R_Montage"));
	if (DEMON_DARK_ATTACK_MONTAGE.Succeeded())
		DemonDarkAttackMontage = DEMON_DARK_ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> DEMON_DARK_SKILL_MONTAGE
	(TEXT("/Game/Assets/Enemies/Demon/Animations/AM_Demon_Tailspin_left_Montage.AM_Demon_Tailspin_left_Montage"));
	if (DEMON_DARK_ATTACK_MONTAGE.Succeeded())
		DemonDarkSkillMontage = DEMON_DARK_ATTACK_MONTAGE.Object;

	// ghoul abyss
	static ConstructorHelpers::FObjectFinder <UAnimMontage> GHOUL_ABYSS_ATTACK_MONTAGE
	(TEXT("/Game/Assets/Enemies/Ghoul/Animations/AM_Attack_L_Montage.AM_Attack_L_Montage"));
	if (GHOUL_ABYSS_ATTACK_MONTAGE.Succeeded())
		GhoulAbyssAttackMontage = GHOUL_ABYSS_ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> GHOUL_ABYSS_SKILL_MONTAGE
	(TEXT("/Game/Assets/Enemies/Ghoul/Animations/AM_Attack_R_Montage.AM_Attack_R_Montage"));
	if (GHOUL_ABYSS_SKILL_MONTAGE.Succeeded())
		GhoulAbyssSkillMontage = GHOUL_ABYSS_SKILL_MONTAGE.Object;

	// goblin spear
	static ConstructorHelpers::FObjectFinder <UAnimMontage> GOBLIN_SPEAR_ATTACK_MONTAGE
	(TEXT("/Game/Assets/Enemies/Goblin/Animations/Spear/AM_Attack_Spear_Montage.AM_Attack_Spear_Montage"));
	if (GOBLIN_SPEAR_ATTACK_MONTAGE.Succeeded())
		GoblinSpearAttackMontage = GOBLIN_SPEAR_ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> GOBLIN_SPEAR_SKILL_MONTAGE
	(TEXT("/Game/Assets/Enemies/Goblin/Animations/Spear/AM_Skill_Spear_Montage.AM_Skill_Spear_Montage"));
	if (GOBLIN_SPEAR_SKILL_MONTAGE.Succeeded())
		GoblinSpearSkillMontage = GOBLIN_SPEAR_SKILL_MONTAGE.Object;

	// goblin slingshot
	static ConstructorHelpers::FObjectFinder <UAnimMontage> GOBLIN_SLINGSHOT_ATTACK_MONTAGE
	(TEXT("/Game/Assets/Enemies/Goblin/Animations/Slingshot/AM_Attack_Slingshot_Montage.AM_Attack_Slingshot_Montage"));
	if (GOBLIN_SLINGSHOT_ATTACK_MONTAGE.Succeeded())
		GoblinSlingshotAttackMontage = GOBLIN_SLINGSHOT_ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> GOBLIN_SLINGSHOT_SKILL_MONTAGE
	(TEXT("/Game/Assets/Enemies/Goblin/Animations/Slingshot/AM_Skill_Slingshot_Montage.AM_Skill_Slingshot_Montage"));
	if (GOBLIN_SLINGSHOT_SKILL_MONTAGE.Succeeded())
		GoblinSlingshotSkillMontage = GOBLIN_SLINGSHOT_SKILL_MONTAGE.Object;

	// goblin shaman
	static ConstructorHelpers::FObjectFinder <UAnimMontage> GOBLIN_SHAMAN_ATTACK_MONTAGE
	(TEXT("/Game/Assets/Enemies/Shaman/Animations/AM_Shaman_Attack_Montage.AM_Shaman_Attack_Montage"));
	if (DEMON_DARK_ATTACK_MONTAGE.Succeeded())
		GoblinShamanAttackMontage = DEMON_DARK_ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> GOBLIN_SHAMAN_SKILL_MONTAGE
	(TEXT("/Game/Assets/Enemies/Shaman/Animations/AM_Shaman_Summon_Montage.AM_Shaman_Summon_Montage"));
	if (DEMON_DARK_ATTACK_MONTAGE.Succeeded())
		GoblinShamanSkillMontage = DEMON_DARK_ATTACK_MONTAGE.Object;

	// shroom poison
	static ConstructorHelpers::FObjectFinder <UAnimMontage> SHROOM_POISON_ATTACK_MONTAGE
	(TEXT("/Game/Assets/Enemies/Shroom/Animations/AM_Attack_Montage.AM_Attack_Montage"));
	if (SHROOM_POISON_ATTACK_MONTAGE.Succeeded())
		ShroomPoisonAttackMontage = SHROOM_POISON_ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> SHROOM_POISON_SKILL_MONTAGE
	(TEXT("/Game/Assets/Enemies/Shroom/Animations/AM_Bounce_Montage.AM_Bounce_Montage"));
	if (SHROOM_POISON_SKILL_MONTAGE.Succeeded())
		ShroomPoisonSkillMontage = SHROOM_POISON_SKILL_MONTAGE.Object;

}

void UAC_AnimInstance::PlayAttackMontage(const FString& key)
{
	SetPlayAttackMontage(key);

	Montage_Play(AttackMontage);
}

void UAC_AnimInstance::PlaySkillMontage(const FString& key)
{
	SetPlaySkillMontage(key);

	Montage_Play(SKillMontage);
}

void UAC_AnimInstance::SetPlayAttackMontage(const FString& key)
{
	if (key.Contains(TEXT("DemonDark")))
		AttackMontage = DemonDarkAttackMontage;

	if (key.Contains(TEXT("DemonRed")))
		AttackMontage = DemonRedAttackMontage;

	if (key.Contains(TEXT("GhoulAbyss")))
		AttackMontage = GhoulAbyssAttackMontage;

	if (key.Contains(TEXT("GoblinShaman")))
		AttackMontage = GoblinShamanAttackMontage;

	if (key.Contains(TEXT("GoblinSlingshot")))
		AttackMontage = GoblinSlingshotAttackMontage;

	if (key.Contains(TEXT("GoblinSpear")))
		AttackMontage = GoblinSpearAttackMontage;

	if (key.Contains(TEXT("ShroomPoison")))
		AttackMontage = ShroomPoisonAttackMontage;
}

void UAC_AnimInstance::SetPlaySkillMontage(const FString& key)
{
	if (key.Contains(TEXT("DemonDark")))
		SKillMontage = DemonDarkSkillMontage;

	if (key.Contains(TEXT("DemonRed")))
		SKillMontage = DemonRedSkillMontage;

	if (key.Contains(TEXT("GhoulAbyss")))
		SKillMontage = GhoulAbyssSkillMontage;

	if (key.Contains(TEXT("GoblinShaman")))
		SKillMontage = GoblinShamanSkillMontage;

	if (key.Contains(TEXT("GoblinSlingshot")))
		SKillMontage = GoblinSlingshotSkillMontage;

	if (key.Contains(TEXT("GoblinSpear")))
		SKillMontage = GoblinSpearSkillMontage;

	if (key.Contains(TEXT("ShroomPoison")))
		SKillMontage = ShroomPoisonSkillMontage;
}
