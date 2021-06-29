// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteSpeedGameModeBase.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "BaseSaveGame.h"
#include "Kismet/GameplayStatics.h"



void AInfiniteSpeedGameModeBase::BeginPlay() {
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

}

void AInfiniteSpeedGameModeBase::SpawnNextActor(FVector SpawningLocation) {

	if (SpawningActorClass == nullptr) return;

	FActorSpawnParameters SpawnParameters;

	GetWorld()->SpawnActor<AActor>(SpawningActorClass, SpawningLocation, FRotator::ZeroRotator, SpawnParameters);
	UE_LOG(LogTemp, Warning, TEXT("Actor is spawned!"));
}

void AInfiniteSpeedGameModeBase::GameEnd(float Speed)
{
	UE_LOG(LogTemp, Warning, TEXT("End Speed : %f"), Speed);

	CurrentSpeed = Speed;

	LoadGame();

	if (CurrentSpeed > HighSpeed) {
		SaveGame();
		HighSpeed = CurrentSpeed;
	}

	if (GameEndWidgetClass) {

		GameEndWidget = CreateWidget<UUserWidget>(GetWorld(), GameEndWidgetClass);
		if (GameEndWidget) {

			GameEndWidget->AddToViewport();

			TextBlockHighSpeed = Cast<UTextBlock>(GameEndWidget->GetWidgetFromName(TEXT("TextBlock_HighSpeed")));
			TextBlockLastSpeed = Cast<UTextBlock>(GameEndWidget->GetWidgetFromName(TEXT("TextBlock_LastSpeed")));

			if (TextBlockHighSpeed)
				TextBlockHighSpeed->SetText(FText::FromString("HIGH SPEED : " + FString::FromInt(HighSpeed)));
			
			if (TextBlockLastSpeed)
				TextBlockLastSpeed->SetText(FText::FromString("CURRENT SPEED : " + FString::FromInt(CurrentSpeed)));

		}
	}
}

void AInfiniteSpeedGameModeBase::SaveGame()
{
	UBaseSaveGame* SaveGameInstance = Cast<UBaseSaveGame>(UGameplayStatics::CreateSaveGameObject(UBaseSaveGame::StaticClass()));
	SaveGameInstance->HighSpeed = CurrentSpeed;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Slot"), 0);
}

void AInfiniteSpeedGameModeBase::LoadGame()
{
	UBaseSaveGame* SaveGameInstance = Cast<UBaseSaveGame>(UGameplayStatics::CreateSaveGameObject(UBaseSaveGame::StaticClass()));
	SaveGameInstance = Cast<UBaseSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot"), 0));

	if(SaveGameInstance)
		HighSpeed = SaveGameInstance->HighSpeed;

}
