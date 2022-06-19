// Fill out your copyright notice in the Description page of Project Settings.


#include "LKPathGuide.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"
#include "LKCharacter.h"

// Sets default values
ALKPathGuide::ALKPathGuide()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AIControllerClass = AMyPathGuideAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DestLocation = FVector::ZeroVector;
	SetReplicates(true);
	SetReplicateMovement(true);

	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeadPawn"));
}

void ALKPathGuide::SetDestination(const FVector NewDestination)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	DestLocation = NewDestination;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), NewDestination);
}

void ALKPathGuide::SetDestTarget(ACharacter * NewDestTarget)
{
	DestTarget = NewDestTarget;
}

// Called when the game starts or when spawned
void ALKPathGuide::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALKPathGuide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();
	//FVector DestLocation = FVector(100.0f, 100.0f, 100.0f);
	float const Distance = FVector::Dist(Location, DestLocation);
	if (Distance < 100.0f)
	{
		//BALOG(Warning, TEXT("MyPathGuidCharacter arrived"));
		//Destroy();

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		//------------------------------------------------------------------------------|
		//after actor reached, check distance to target and then set destination again. 
		//------------------------------------------------------------------------------|
		ALKCharacter* Target = Cast<ALKCharacter>(DestTarget);
		if (nullptr != DestTarget && Target->IsDead())
		{
			DestTarget = nullptr;
		}

		if (nullptr != DestTarget)
		{
			float const DistanceToTarget = FVector::Dist(Location, DestTarget->GetActorLocation());
			if (DistanceToTarget >= 100.0f)
			{
				SetDestination(DestTarget->GetActorLocation());
			}
		}
	}
}



void ALKPathGuide::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALKPathGuide, OwnerPlayerId)
	DOREPLIFETIME(ALKPathGuide, OwnerPCName)
		

}
