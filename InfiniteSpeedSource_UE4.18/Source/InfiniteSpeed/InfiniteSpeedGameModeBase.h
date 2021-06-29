// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfiniteSpeedGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class INFINITESPEED_API AInfiniteSpeedGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
public:

	void SpawnNextActor(FVector SpawningLocation);

	void GameEnd(float Speed);
	
private:

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> SpawningActorClass;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> GameEndWidgetClass;

	class UUserWidget* GameEndWidget;

	class UTextBlock* TextBlockHighSpeed;
	class UTextBlock* TextBlockLastSpeed;

	float CurrentSpeed;
	float HighSpeed;

	void SaveGame();
	void LoadGame();

};
