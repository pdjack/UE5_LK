// Fill out your copyright notice in the Description page of Project Settings.


#include "LKGameModeBase.h"
#include "LKCharacter.h"
#include "LKPlayerController.h"
#include "LKGameInstance.h"
#include "LKPathGuide.h"
#include "LKPet.h"
#include "GameFramework/PlayerState.h"

ALKGameModeBase::ALKGameModeBase()
{
	DefaultPawnClass = ALKCharacter::StaticClass();
	PlayerControllerClass = ALKPlayerController::StaticClass();
	PathGuideClass = ALKPathGuide::StaticClass();
	PetClass = ALKPet::StaticClass();
}

void ALKGameModeBase::PostLogin(APlayerController * NewPlayer)
{
	UE_LOG(LogTemp, Log, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("PostLogin End"));

	auto Pawn = Cast<ALKCharacter>(NewPlayer->GetPawn());
	//FString Name = Cast<ULKGameInstance>(GetGameInstance())->;
	uint32 uID = Pawn->GetUniqueID();
	Pawn->SetServerUniqueID(uID);
	UE_LOG(LogTemp, Log, TEXT("PostLogin uID:%d"), uID);
}

void ALKGameModeBase::Logout(AController * Exiting)
{	
	Super::Logout(Exiting);
	
	auto PC = Cast<ALKPlayerController>(Exiting);
	if (PC)
	{
		LKLOG(Warning, TEXT("Logout Name : %s"), *PC->GetName());
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PathGuideClass, FoundActors);
		LKLOG(Warning, TEXT("PathGuides Num : %d"), FoundActors.Num());
		for (int i = 0; i < FoundActors.Num(); ++i)
		{
			auto Temp = Cast<ALKPathGuide>(FoundActors[i]);

			if (nullptr != Temp)
			{
				if (Temp->OwnerPCName.Equals(PC->GetName()))
				{
					//int32 OwnerPlayerId = Temp->OwnerPlayerId;
					LKLOG(Warning, TEXT("Destroy PathGuid Actor in World =  Owner PC Name : %s"), *Temp->OwnerPCName);
					Temp->Destroy();
				}

			}
		}

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PathGuideClass, FoundActors);
		LKLOG(Warning, TEXT("after detroy, PathGuides Num : %d"), FoundActors.Num());


		TArray<AActor*> FoundPets;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PetClass, FoundPets);
		LKLOG(Warning, TEXT("PetClass Num : %d"), FoundPets.Num());
		for (int i = 0; i < FoundPets.Num(); ++i)
		{
			auto Temp = Cast<ALKPet>(FoundPets[i]);

			if (nullptr != Temp)
			{
				if (Temp->OwnerPCName.Equals(PC->GetName()))
				{
					//int32 OwnerPlayerId = Temp->OwnerPlayerId;
					LKLOG(Warning, TEXT("Destroy Pet Actor in World =  Owner PC Name : %s"), *Temp->OwnerPCName);
					Temp->Destroy();
				}

			}
		}

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PetClass, FoundPets);
		LKLOG(Warning, TEXT("after detroy , PetClass Num : %d"), FoundPets.Num());
	}

}
