// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawningActor.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "InfiniteSpeedGameModeBase.h"
#include "BasePawn.h"
#include "Engine/World.h"


// Sets default values
ASpawningActor::ASpawningActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMeshComp"));
	BaseMeshComp->SetupAttachment(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);

	SpawningDistance = 200.0f;
	ObstacleBorderY = 300.0f;

	
}

// Called when the game starts or when spawned
void ASpawningActor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASpawningActor::OnComponentOverlapBegin);

	GM = Cast<AInfiniteSpeedGameModeBase>(GetWorld()->GetAuthGameMode());


	if (ObstacleActor) {

		FVector SpawnLocation = GetActorLocation() + FVector(0.0f, -300.0f, 0.0f);
		int32 ObstacleCount = 0;

		for (int32 i = 0; i < 5; i++)
		{

			SpawnLocation += FVector(0.0f, 100.0f, 0.0f);

			if (FMath::RandRange(0, 1) == 0) {


				FActorSpawnParameters SpawnParameters;

				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ObstacleActor, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);

				if (SpawnedActor) {
					SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					SpawnedActor->SetLifeSpan(30.0f);
				}


				ObstacleCount++;
			}


			if (ObstacleCount == 3) {

				break;
			}

		}


	}


	if (ObstacleBorderActor) {

		AActor* Obstacle1 = GetWorld()->SpawnActor<AActor>(ObstacleBorderActor, GetActorLocation() + FVector(0.0f, -ObstacleBorderY, ObstacleBorderZ), FRotator::ZeroRotator);

		if (Obstacle1) {
			Obstacle1->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Obstacle1->SetLifeSpan(30.0f);
		}

		AActor* Obstacle2 = GetWorld()->SpawnActor<AActor>(ObstacleBorderActor, GetActorLocation() + FVector(0.0f, ObstacleBorderY, ObstacleBorderZ), FRotator::ZeroRotator);

		if (Obstacle2) {
			Obstacle2->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Obstacle2->SetLifeSpan(30.0f);
		}
	}

	bIsOverlapped = false;
}

// Called every frame
void ASpawningActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawningActor::OnComponentOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ABasePawn>(OtherActor) == nullptr || bIsOverlapped) return;

	if (GM) {
		GM->SpawnNextActor(GetActorLocation() + FVector(SpawningDistance, 0.0f, 0.0f));
		
		bIsOverlapped = true;

		SetLifeSpan(10.0f);
	}

}

