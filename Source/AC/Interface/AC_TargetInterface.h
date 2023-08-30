// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AC_TargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAC_TargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AC_API IAC_TargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
