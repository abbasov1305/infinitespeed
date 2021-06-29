// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class INFINITESPEED_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowComp;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> MainWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* CurrentWidget;

	UPROPERTY(EditAnywhere, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> MenuWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* MenuWidget;


	UPROPERTY()
	class UTextBlock* TextBlockSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float DefaultSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SpeedRate;
	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MaxRotation;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationRestoreSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MinRestoreAngle;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float OffsetX;
	FVector NextLocation;
	

	bool bRestoreRotation;
	bool bIsGameEnd;
	bool bIsGameStarted;

	
	UPROPERTY()
	class AInfiniteSpeedGameModeBase* GM;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	AActor* MainCamera;

	void MoveForward(float DeltaTime);
	void MoveRight(float Value);
	void RestoreRotation();

	void StartGame();
	void PlayAgin();
	void QuitGame();

public:

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
};
