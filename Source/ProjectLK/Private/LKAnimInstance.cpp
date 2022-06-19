// Fill out your copyright notice in the Description page of Project Settings.


#include "LKAnimInstance.h"
#include "LKCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
ULKAnimInstance::ULKAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	IsDancing = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void ULKAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		/*auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}*/

		auto LKCharacter = Cast<ALKCharacter>(Pawn);
		if (LKCharacter)
		{
			IsInAir = LKCharacter->GetMovementComponent()->IsFalling();
			IsDancing = LKCharacter->IsDancing();
			DanceID = LKCharacter->GeDanceID();
		}

		
	}
}

void ULKAnimInstance::PlayAttackMontage()
{
	LKCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
	
}

void ULKAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	LKCHECK(!IsDead);
	if (!Montage_IsPlaying(AttackMontage))
	{
		LKLOG(Warning, TEXT("!Montage_IsPlaying(AttackMontage):%d"), NewSection);
	}
	
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection),AttackMontage);
}

void ULKAnimInstance::AnimNotify_AttackHitCheck()
{
	//LKLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}

void ULKAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName ULKAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	LKCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	LKLOG(Warning, TEXT("Attack%d"), Section);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
