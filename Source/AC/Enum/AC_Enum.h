// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Skill UMETA(DisplayName = "Skill"),
	Dead UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class EUIType : uint8
{
	ChampionCardUI UMETA(DisplayName = "ChampionCardUI"),
	ChampionStoreUI UMETA(DisplayName = "ChampionStoreUI"),
};

#define NormalMaxLevel 6
#define ExtendedMaxLevel 7

#define TyleSizeFloor1x1 100

