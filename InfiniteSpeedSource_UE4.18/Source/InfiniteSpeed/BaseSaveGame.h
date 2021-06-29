// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BaseSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class INFINITESPEED_API UBaseSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	
public:

	UBaseSaveGame();

	UPROPERTY()
	float HighSpeed;
	
};
