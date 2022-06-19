// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectLK.h"
#include "GameFramework/GameModeBase.h"
#include "LKGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLK_API ALKGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
		
	ALKGameModeBase();
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	//UPROPERTY(EditDefaultsOnly, Category = PathGuideClass)
	TSubclassOf<class ALKPathGuide> PathGuideClass;
	TSubclassOf<class ALKPet> PetClass;
};
