// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "InfiniteSpeedGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(RootComponent);
	ArrowComp->SetArrowColor(FLinearColor::Green);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 600.0f;
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bEnableCameraLag = true;

	//CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	//CameraComp->SetupAttachment(SpringArmComp);


	DefaultSpeed = 100.0f;
	RotationSpeed = 10.0f;
	SpeedRate = 1.0f;
	MaxRotation = 45.0f;
	RotationRestoreSpeed = 100.0f;
	MinRestoreAngle = 10.0f;
	OffsetX = -100.0f;

	bRestoreRotation = false;
	bIsGameEnd = false;
	bIsGameStarted = false;
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentSpeed = DefaultSpeed;

	if (MenuWidgetClass) {
		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);

		if (MenuWidget)
			MenuWidget->AddToViewport();

	}

	if (MainWidgetClass) {

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
		if (CurrentWidget) {
			//CurrentWidget->AddToViewport();

			TextBlockSpeed = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("TextBlock_Speed")));

			if (TextBlockSpeed) {
				TextBlockSpeed->SetText(FText::FromString(FString::FromInt(CurrentSpeed)));
			}
		}
	}

	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClass(this, ACameraActor::StaticClass(), CameraActors);

	for (int32 i = 0; i < CameraActors.Num(); i++) {

		if (CameraActors[i]->ActorHasTag(TEXT("MainCamera"))) {

			MainCamera = CameraActors[i];

			if (MainCamera) {

				GetWorld()->GetFirstPlayerController()->SetViewTarget(MainCamera);
				NextLocation = MainCamera->GetActorLocation();

				break;
			}
		}
	}

	

	GM = Cast<AInfiniteSpeedGameModeBase>(GetWorld()->GetAuthGameMode());

	bIsGameStarted = false;
	bRestoreRotation = false;
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGameEnd || !bIsGameStarted) return;

	MoveForward(DeltaTime);

	if (bRestoreRotation) {
		
		FRotator TargetRot;

		if (GetActorRotation().Yaw < -MinRestoreAngle) {

			TargetRot = FRotator(0.0f, RotationRestoreSpeed * DeltaTime, 0.0f);
			//AddActorLocalRotation(FRotator(0.0f, RotationRestoreSpeed * DeltaTime, 0.0f));
		}
		else if (GetActorRotation().Yaw > MinRestoreAngle) {

			TargetRot = FRotator(0.0f, -RotationRestoreSpeed * DeltaTime, 0.0f);
			//AddActorLocalRotation(FRotator(0.0f, -RotationRestoreSpeed * DeltaTime, 0.0f));
		}
		else {

			bRestoreRotation = false;
			return;
		}
		
		AddActorLocalRotation(TargetRot);
	}
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &ABasePawn::MoveRight);
	PlayerInputComponent->BindAction("MoveRight", IE_Released, this, &ABasePawn::RestoreRotation);

	PlayerInputComponent->BindAction("StartGame", IE_Pressed, this, &ABasePawn::StartGame);
	PlayerInputComponent->BindAction("PlayAgin", IE_Pressed, this, &ABasePawn::PlayAgin);
	PlayerInputComponent->BindAction("QuitGame", IE_Pressed, this, &ABasePawn::QuitGame);

}

void ABasePawn::MoveForward(float DeltaTime)
{
	CurrentSpeed += SpeedRate * DeltaTime;

	AddActorLocalOffset(FVector(CurrentSpeed * DeltaTime, 0.0f, 0.0f), true);
	
	if (MainCamera) {

		NextLocation.X = GetActorLocation().X + OffsetX;
		MainCamera->SetActorLocation(NextLocation);
	}

	if (TextBlockSpeed) {
		TextBlockSpeed->SetText(FText::FromString(FString::FromInt(CurrentSpeed)));
	}
}

void ABasePawn::MoveRight(float Value)
{
	if (Value == 0.0f || FMath::Abs(GetActorRotation().Yaw) > MaxRotation || bIsGameEnd) return;


	//UE_LOG(LogTemp, Warning, TEXT("Player yaw rotation : %f"), GetActorRotation().Yaw);
	CurrentSpeed -= SpeedRate * 2.0f * GetWorld()->DeltaTimeSeconds;
	
	AddActorLocalRotation(FRotator(0.0f, Value * RotationSpeed, 0.0f), true);

	bRestoreRotation = false;
	
}

void ABasePawn::RestoreRotation()
{
	bRestoreRotation = true;

	//AddActorLocalRotation(FRotator(0.0f, -GetActorRotation().Yaw, 0.0f));
}


void ABasePawn::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(TEXT("Obstacle"))) {

		UE_LOG(LogTemp, Error, TEXT("Game Over"));
		bIsGameEnd = true;

		if (GM) {

			GM->GameEnd(CurrentSpeed);

			if (CurrentWidget)
				CurrentWidget->RemoveFromViewport();
		}
	}
}


void ABasePawn::StartGame()
{
	if (bIsGameStarted) return;

	bIsGameStarted = true;


	if (MenuWidget && CurrentWidget) {

		MenuWidget->RemoveFromViewport();
		CurrentWidget->AddToViewport();
	}
}

void ABasePawn::PlayAgin()
{


	GetWorld()->GetFirstPlayerController()->RestartLevel();
}

void ABasePawn::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit);
}
