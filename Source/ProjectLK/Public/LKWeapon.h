// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectLK.h"
#include "GameFramework/Actor.h"
#include "LKWeapon.generated.h"

UCLASS()
class PROJECTLK_API ALKWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALKWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMesh(int32 WeaponNum);
public :
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;


protected:
	UFUNCTION()
	void OnAssetLoadCompleted();

private:
	FSoftObjectPath WeaponAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
};
