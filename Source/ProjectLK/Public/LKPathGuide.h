// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectLK.h"
#include "GameFramework/Character.h"
#include "LKPathGuide.generated.h"

UCLASS()
class PROJECTLK_API ALKPathGuide : public ACharacter
{
	GENERATED_BODY()
public :
	ALKPathGuide();

	void SetDestination(const FVector NewDestination);

	UPROPERTY(Replicated)
	int32 OwnerPlayerId = -1;

	UPROPERTY(Replicated)
	FString OwnerPCName; //OwnerPawn 이 logout(GameMode) 할때 삭제 하기 위해 사용

	void SetDestTarget(ACharacter* NewDestTarget);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector DestLocation;

	ACharacter* DestTarget;

};
