// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Attack UMETA(DisplayName = "Attack"),
	Skill UMETA(DisplayName = "Skill"),
	Dead UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class EUIType : uint8
{
	ChampionCardUI UMETA(DisplayName = "ChampionCardUI"),
	ChampionStoreUI UMETA(DisplayName = "ChampionStoreUI"),
	GameRoundUI UMETA(DisplayName = "GameRoundUI"),
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Prepare UMETA(DisplayName = "Prepare"),
	Ready UMETA(DisplayName = "Ready"),
	Battle UMETA(DisplayName = "Battle"),
};

#define NormalMaxLevel 6
#define ExtendedMaxLevel 7

#define TyleSizeFloor1x1 100

#define Def_WaitingSeat1234 0
#define Def_WaitingSeat5678 1

#define Def_ArenaLeftDown 0
#define Def_ArenaLeftUp 1
#define Def_ArenaRightDown 2
#define Def_ArenaRightUp 3

#define Def_NS_Fire1 0
#define Def_NS_Fire2 1
#define Def_NS_Fire3 2
#define Def_NS_Fire4 3
#define Def_NS_Fire5 4

#define TimeLimit_Prepare 7
#define TimeLimit_Ready 3
#define TimeLimit_Battle 15


