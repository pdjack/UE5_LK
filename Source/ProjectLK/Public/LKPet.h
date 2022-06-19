// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectLK.h"
#include "GameFramework/Character.h"
#include "LKPet.generated.h"

UCLASS()
class PROJECTLK_API ALKPet : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALKPet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//서버에서만 호출되야 함
	void SetAssetNum(int32 NewAsset);

	void SetMeshScale3D(float size);

public:
	//UPROPERTY(VisibleAnywhere, Category = Weapon)
	//USkeletalMeshComponent* Pet;

	UPROPERTY(Replicated)
	int32 OwnerPlayerId = -1;

	UPROPERTY(Replicated)
	FString OwnerPCName; //OwnerPawn 이 logout(GameMode) 할때 삭제 하기 위해 사용

	void SetDestTarget(ACharacter* NewDestTarget);
protected:
	UFUNCTION()
	void OnAssetLoadCompleted();


	UPROPERTY(ReplicatedUsing = OnRep_PetAsset, BlueprintReadOnly, Category = "Player")
	int32 PetAssetNum;

	UFUNCTION()
	void OnRep_PetAsset();

private:
	FSoftObjectPath PetAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	ACharacter* DestTarget;

private:
	void SetMesh(int32 NewAssetNum);

};

