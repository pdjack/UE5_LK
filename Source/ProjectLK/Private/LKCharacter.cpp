// Fill out your copyright notice in the Description page of Project Settings.


#include "LKCharacter.h"
#include "LKAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "EngineGlobals.h"
#include "Engine/NetworkObjectList.h"
#include "EngineUtils.h"
#include "LKPathGuide.h"
#include "GameFramework/PlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "DrawDebugHelpers.h"
#include "LKWeapon.h"
#include "LKGameInstance.h"
#include "LKCharacterSetting.h"
#include "LKCharacterStatComponent.h"

#include "Components/WidgetComponent.h"
#include "LKCharacterWidget.h"

#include "LKPet.h"

// Sets default values
ALKCharacter::ALKCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<ULKCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	CharacterNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PLAYERNAMEWIDGET"));
	

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	CharacterNameWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));


	
	

	CharacterNameWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	CharacterNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	//WidgetBlueprint'/Game/Blueprints/UI_CharacterNameOnHead.UI_CharacterNameOnHead'
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_PLAYERNAME(TEXT("/Game/0_ProjectLK/Blueprints/UI_CharacterNameOnHead.UI_CharacterNameOnHead_C"));
	if (UI_PLAYERNAME.Succeeded())
	{
		CharacterNameWidget->SetWidgetClass(UI_PLAYERNAME.Class);
		CharacterNameWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	//------------------AnimInstance------------------//
	
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> MALE_ANIM(TEXT("/Game/SurvivalMan/Demo/Animation/SurvMan_AnimBP.SurvMan_AnimBP_C"));
	if (MALE_ANIM.Succeeded())
	{
		MALE_ANIM_CLASS = MALE_ANIM.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> FEMALE_ANIM(TEXT("/Game/SurvivalGirl/Animations/SurvGirl_AnimBP.SurvGirl_AnimBP_C"));
	if (FEMALE_ANIM.Succeeded())
	{
		//GetMesh()->SetAnimInstanceClass(FEMALE_ANIM.Class);
		FEMALE_ANIM_CLASS = FEMALE_ANIM.Class;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> MALE_CITIZEN_ANIM(TEXT("/Game/SurvivalMan/Demo/Animation/MaleCitizen_AnimBP.MaleCitizen_AnimBP_C"));
	if (MALE_CITIZEN_ANIM.Succeeded())
	{
		MALE_CITIZEN_ANIM_CLASS = MALE_CITIZEN_ANIM.Class;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> FEMALE_CITIZEN_ANIM(TEXT("/Game/SurvivalGirl/Animations/FamaleCitizen_AnimBP.FamaleCitizen_AnimBP_C"));
	if (FEMALE_CITIZEN_ANIM.Succeeded())
	{
		//GetMesh()->SetAnimInstanceClass(FEMALE_ANIM.Class);
		FEMALE_CITIZEN_ANIM_CLASS = FEMALE_CITIZEN_ANIM.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> FEMALE_SCHOOLGIRL_ANIM(TEXT("/Game/SchoolGirls/Demo/Animations/SchoolGirs_AnimBP.SchoolGirs_AnimBP_C"));
	if (FEMALE_SCHOOLGIRL_ANIM.Succeeded())
	{
		FEMALE_SCHOOLGIRL_ANIM_CLASS = FEMALE_SCHOOLGIRL_ANIM.Class;
	}
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	


	


	//---------------Parts-------------------//
	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_HAIR"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_HAIR(TEXT("/Game/0_ProjectLK/Custom/Citizen_NPC/Sk_Citizen_NPC_Hair_1.Sk_Citizen_NPC_Hair_1"));
	if (SK_HAIR.Succeeded())
	{
		Hair->SetSkeletalMesh(SK_HAIR.Object);
	}

	Hair->SetCollisionProfileName(TEXT("NoCollision"));
	Hair->SetupAttachment(GetMesh());
	Hair->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	

	Top = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_TOP"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_TOP(TEXT("/Game/0_ProjectLK/Custom/Citizen_NPC/Sk_Citizen_NPC_16_1.Sk_Citizen_NPC_16_1"));
	if (SK_TOP.Succeeded())
	{
		Top->SetSkeletalMesh(SK_TOP.Object);
	}

	Top->SetCollisionProfileName(TEXT("NoCollision"));
	Top->SetupAttachment(GetMesh());
	Top->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_PANTS"));
	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PANTS(TEXT("/Game/0_ProjectLK/Custom/Citizen_NPC/Sk_Citizen_NPC_16_1.Sk_Citizen_NPC_16_1"));
	if (SK_PANTS.Succeeded())
	{
		Top->SetSkeletalMesh(SK_PANTS.Object);
	}*/

	Pants->SetCollisionProfileName(TEXT("NoCollision"));
	Pants->SetupAttachment(GetMesh());
	Pants->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	

	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_SHOES"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SHOES(TEXT("/Game/0_ProjectLK/Custom/Citizen_NPC/Sk_Citizen_NPC__Heel.Sk_Citizen_NPC__Heel"));
	if (SK_SHOES.Succeeded())
	{
		Shoes->SetSkeletalMesh(SK_SHOES.Object);
	}

	Shoes->SetCollisionProfileName(TEXT("NoCollision"));
	Shoes->SetupAttachment(GetMesh());
	Shoes->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	Onepiece = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_ONEPIECE"));
	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ONEPIECE(TEXT("/Game/0_ProjectLK/Custom/Citizen_NPC/Sk_Citizen_NPC__Heel.Sk_Citizen_NPC__Heel"));
	if (SK_ONEPIECE.Succeeded())
	{
		Onepiece->SetSkeletalMesh(SK_SHOES.Object);
	}*/

	Onepiece->SetCollisionProfileName(TEXT("NoCollision"));
	Onepiece->SetupAttachment(GetMesh());
	Onepiece->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	




	SetControlMode(EControlMode::GTA);

	IsAttacking = false;

	MaxCombo = 4;
	AttackEndComboState();

	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	WeaponClass = ALKWeapon::StaticClass();
	PetClass = ALKPet::StaticClass();

	CharacterAssetID = -1;
	WeaponAssetID = -1;

	//캐릭터 클릭해서 이동할때 사용한다.
	ServerUniqueID = 0;

	CharacterName.Empty();

	Speed = 0.4f;
}

void ALKCharacter::ChangeCharacterName(FString NewCharacterName)
{
	ServerChangeCharacterName(NewCharacterName);
}

void ALKCharacter::SetPetScale3D(float size)
{
	if (CurPet)
	{
		CurPet->SetMeshScale3D(size);
	}
}

// Called when the game starts or when spawned
void ALKCharacter::BeginPlay()
{
	Super::BeginPlay();

	LKLOG(Warning, TEXT("my uniqueID : %d"), GetUniqueID());

	if (IsLocallyControlled())
	{
		
		ServerInitCharacterAsset();
	}
	
	
}


void ALKCharacter::UpdateCharacterNameWidget()
{
	if (IsAlreadySetCharacterName) return;

	auto CharacterWidget = Cast<ULKCharacterWidget>(CharacterNameWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget && IsCharacterNameReplicated && CharacterWidget->IsSetCharacterNameWidget())
	{
		IsAlreadySetCharacterName = true;
		CharacterWidget->SetCharacterName(CharacterName);

	}
}

void ALKCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		SpringArm->TargetArmLength = 450.0f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DIABLO:
		SpringArm->TargetArmLength = 800.0f;
		SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	}
}

void ALKCharacter::SetServerUniqueID(uint32 uID)
{
	ServerUniqueID = (int32)uID;
}

// Called every frame
void ALKCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		FollowPathGuideProcess();


		switch (CurrentControlMode)
		{
		case EControlMode::DIABLO:
			if (DirectionToMove.SizeSquared() > 0.0f)
			{
				GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
				AddMovementInput(DirectionToMove);
			}
			break;
		}
	}


	

	
	
	UpdateCharacterNameWidget();
	
	
}

// Called to bind functionality to input
void ALKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &ALKCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ALKCharacter::JumpHook);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ALKCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("TurnView"), EInputEvent::IE_Pressed, this, &ALKCharacter::TurnViewPressed);
	PlayerInputComponent->BindAction(TEXT("TurnView"), EInputEvent::IE_Released, this, &ALKCharacter::TurnViewReleased);
	
	PlayerInputComponent->BindAction(TEXT("Dance"), EInputEvent::IE_Pressed, this, &ALKCharacter::Dance);
	PlayerInputComponent->BindAction(TEXT("ChangeAvatarType"), EInputEvent::IE_Pressed, this, &ALKCharacter::ChangeAvatarType);


	PlayerInputComponent->BindAction("InputClick", IE_Pressed, this, &ALKCharacter::InputClickPressed);
	PlayerInputComponent->BindAction("InputClick", IE_Released, this, &ALKCharacter::InputClickReleased);

	PlayerInputComponent->BindAction("ChangeCharacterAsset", IE_Pressed, this, &ALKCharacter::ChangeCharacterAsset);
	PlayerInputComponent->BindAction("ChangeWeaponAsset", IE_Pressed, this, &ALKCharacter::ChangeWeaponAsset);
	

	PlayerInputComponent->BindAction("CreatePetAsset", IE_Pressed, this, &ALKCharacter::CreatePetAsset);
	PlayerInputComponent->BindAction("DestroyPetAsset", IE_Pressed, this, &ALKCharacter::DestroyPetAsset);


	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ALKCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ALKCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ALKCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ALKCharacter::Turn);



}

float ALKCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	LKLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	if (FinalDamage > 0.0f)
	{
		bDied = true;
		LKAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}

	return FinalDamage;
}

void ALKCharacter::ChangeCharacterAsset()
{
	ServerInitCharacterAsset();
}

void ALKCharacter::ChangeWeaponAsset()
{
}


void ALKCharacter::CreatePetAsset()
{
	auto DefaultSetting = GetDefault<ULKCharacterSetting>();
	int32 RandIndex = FMath::FRandRange(0, DefaultSetting->PetAssets.Num() - 1);
	ServerCreatePetAsset(RandIndex);
}


void ALKCharacter::DestroyPetAsset()
{
	ServerDestroyPetAsset();
}

void ALKCharacter::AttackStartComboState()
{
	
	//LKLOG(Warning, TEXT("[client]AttackStartComboState"));

	CanNextCombo = true;
	IsComboInputOn = false;
	LKCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
	
	ServerAttack(CurrentCombo);
	
}

void ALKCharacter::AttackEndComboState()
{
	
	//LKLOG(Warning, TEXT("[client]AttackEndComboState"));
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;

	if (IsLocallyControlled())
	{
		
		ServerAttack(CurrentCombo);
	}
}

void ALKCharacter::OnAssetLoadCompleted()
{
	AssetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(CharacterAssetToLoad);

	if (LoadedAssetPath.IsValid())
	{
		//SetSkeltalMesh 호출 전에 AnimInstance 설정 해야 한다.
		SetAnimInstance();
		GetMesh()->SetSkeletalMesh(LoadedAssetPath.Get());

		
	}
}


void ALKCharacter::OnAssetLoadCompleted_PartsOnepiece()
{
	AssetStreamingHandle_PartsOnepiece->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(PartsOnepieceToLoad);

	if (LoadedAssetPath.IsValid())
	{
		//SetSkeltalMesh 호출 전에 AnimInstance 설정 해야 한다.
		SetAnimInstance();
		if (nullptr != Onepiece)
		{
			Onepiece->SetSkeletalMesh(LoadedAssetPath.Get());


			//@todo 임시
			//위 SetAnimInstance 호출시  모든 파츠가 보이기 때문에 Onepiece 와 Top/Pants 가 겹쳐서 보이게 된다. 
			Onepiece->SetHiddenInGame(false);
			Top->SetHiddenInGame(true);
			Pants->SetHiddenInGame(true);
		}
	}
}

void ALKCharacter::OnAssetLoadCompleted_PartsShoes()
{
	AssetStreamingHandle_PartsShoes->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(PartsShoesToLoad);

	if (LoadedAssetPath.IsValid())
	{
		//SetSkeltalMesh 호출 전에 AnimInstance 설정 해야 한다.
		SetAnimInstance();
		if (nullptr != Shoes)
		{
			Shoes->SetSkeletalMesh(LoadedAssetPath.Get());
		}
	}
}

void ALKCharacter::OnAssetLoadCompleted_PartsPants()
{
	AssetStreamingHandle_PartsPants->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(PartsPantsToLoad);

	if (LoadedAssetPath.IsValid())
	{
		//SetSkeltalMesh 호출 전에 AnimInstance 설정 해야 한다.
		SetAnimInstance();
		if (nullptr != Pants)
		{
			Pants->SetSkeletalMesh(LoadedAssetPath.Get());


			//@todo 임시
			//위 SetAnimInstance 호출시  모든 파츠가 보이기 때문에 Onepiece 와 Top/Pants 가 겹쳐서 보이게 된다. 
			Onepiece->SetHiddenInGame(true);
			Top->SetHiddenInGame(false);
			Pants->SetHiddenInGame(false);
		}
	}
}


void ALKCharacter::OnAssetLoadCompleted_PartsTop()
{
	AssetStreamingHandle_PartsTop->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(PartsTopToLoad);

	if (LoadedAssetPath.IsValid())
	{
		//SetSkeltalMesh 호출 전에 AnimInstance 설정 해야 한다.
		SetAnimInstance();
		if (nullptr != Top)
		{
			Top->SetSkeletalMesh(LoadedAssetPath.Get());

			//@todo 임시
			//위 SetAnimInstance 호출시  모든 파츠가 보이기 때문에 Onepiece 와 Top/Pants 가 겹쳐서 보이게 된다. 
			Onepiece->SetHiddenInGame(true);
			Top->SetHiddenInGame(false);
			Pants->SetHiddenInGame(false);
		}
	}
}

void ALKCharacter::OnAssetLoadCompleted_PartsHair()
{
	AssetStreamingHandle_PartsHair->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(PartsHairToLoad);

	if (LoadedAssetPath.IsValid())
	{
		//SetSkeltalMesh 호출 전에 AnimInstance 설정 해야 한다.
		SetAnimInstance();
		if (nullptr != Hair)
		{
			Hair->SetSkeletalMesh(LoadedAssetPath.Get());

			//@todo 임시
			//위 SetAnimInstance 호출시  모든 파츠가 보이기 때문에 Onepiece 와 Top/Pants 가 겹쳐서 보이게 된다. 
			// if (Onepiece->bHiddenInGame) 로 Onepiece 가 보이는 상태인지 아닌지에 따라 처리하려고 했지만, 
			// bHiddenInGame이 같은 값으로만 리턴이 된다. 그래서 아래 주석 처리 하고,  Onepiece 안보이게 함
			/*if (Onepiece->bHiddenInGame)
			{
				Onepiece->SetHiddenInGame(false);
				Top->SetHiddenInGame(true);
				Pants->SetHiddenInGame(true);
			}
			else
			{
				Onepiece->SetHiddenInGame(true);
				Top->SetHiddenInGame(false);
				Pants->SetHiddenInGame(false);
			}*/
			Onepiece->SetHiddenInGame(true);
			Top->SetHiddenInGame(false);
			Pants->SetHiddenInGame(false);
			Shoes->SetHiddenInGame(false);
		}
	}
}

void ALKCharacter::SetAnimInstance()
{
	switch (CurrentAvatarType)
	{
	case (int32)EAvatarType::MALE_DEFAULT: //통짜

		GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		GetMesh()->SetAnimInstanceClass(MALE_ANIM_CLASS);
		
		SetPartsHiddenInGame(true);
		break;

	case (int32)EAvatarType::FEMALE_DEFAULT: //통짜

		GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		GetMesh()->SetAnimInstanceClass(FEMALE_ANIM_CLASS);
		
		SetPartsHiddenInGame(true);
		break;

	case (int32)EAvatarType::FEMALE_SCHOOLGIRL: //통짜

		GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		GetMesh()->SetAnimInstanceClass(FEMALE_SCHOOLGIRL_ANIM_CLASS);

		SetPartsHiddenInGame(true);
		break;
	case (int32)EAvatarType::MALE_CITIZEN: //파츠
	//1.
		GetMesh()->SetAnimInstanceClass(nullptr);
		SetPartsAnimInstanceClass(nullptr);

		GetMesh()->SetRelativeScale3D(FVector(1.14f, 1.14f, 1.14f));
		GetMesh()->SetAnimInstanceClass(MALE_CITIZEN_ANIM_CLASS);

		//2. 1.에서 초기화 하고 나서 세팅 해야 파츠랑 Main Mesh랑 애니메이션이 동기화됨.
		SetPartsAnimInstanceClass(MALE_CITIZEN_ANIM_CLASS);

		SetPartsHiddenInGame(false);
		break;

	
	case (int32)EAvatarType::FEMALE_CITIZEN: //파츠
		//1.
		GetMesh()->SetAnimInstanceClass(nullptr);
		SetPartsAnimInstanceClass(nullptr);

		GetMesh()->SetRelativeScale3D(FVector(1.14f, 1.14f, 1.14f));
		GetMesh()->SetAnimInstanceClass(FEMALE_CITIZEN_ANIM_CLASS);
		
		//2. 1.에서 초기화 하고 나서 세팅 해야 파츠랑 Main Mesh랑 애니메이션이 동기화됨.
		SetPartsAnimInstanceClass(FEMALE_CITIZEN_ANIM_CLASS);
			   		
		SetPartsHiddenInGame(false);
		break;
	
	}
}

void ALKCharacter::SetPartsAnimInstanceClass(class UClass* NewClass)
{
	if (nullptr != Hair)Hair->SetAnimInstanceClass(NewClass);
	if (nullptr != Top)Top->SetAnimInstanceClass(NewClass);
	if (nullptr != Pants)Pants->SetAnimInstanceClass(NewClass);
	if (nullptr != Shoes)Shoes->SetAnimInstanceClass(NewClass);
	if (nullptr != Onepiece)Onepiece->SetAnimInstanceClass(NewClass);
}

void ALKCharacter::SetPartsHiddenInGame(bool hidden)
{
	if (nullptr != Hair)Hair->SetHiddenInGame(hidden);
	if (nullptr != Top)Top->SetHiddenInGame(hidden);
	if (nullptr != Pants)Pants->SetHiddenInGame(hidden);
	if (nullptr != Shoes)Shoes->SetHiddenInGame(hidden);
	if (nullptr != Onepiece)Onepiece->SetHiddenInGame(hidden);
}

void ALKCharacter::InputClickPressed()
{
	bClickMouse = true;
	//BALOG(Warning, TEXT("bClickMouse = true"));
}

void ALKCharacter::InputClickReleased()
{	
	bClickMouse = false;
	//BALOG(Warning, TEXT("bClickMouse = false"));
	if (IsLocallyControlled() && !IsDead())
	{
		MoveToMouseCursor();
	}

}

void ALKCharacter::TurnViewPressed()
{
	bTurnView = true;
}
void ALKCharacter::TurnViewReleased()
{
	bTurnView = false;
}

void ALKCharacter::MoveToMouseCursor()
{
	FHitResult Hit;
	Cast<APlayerController>(GetController())->GetHitResultUnderCursor(ECC_Visibility, false, Hit);


	if (Hit.bBlockingHit)
	{
		//BALOG(Warning, TEXT("Hit Name : %s"), *Hit.Actor->GetName());
		UE_LOG(LogTemp, Log, TEXT("Hit Name : %s"), *Hit.GetActor()->GetName());

		auto HitActor = Cast<ALKCharacter>(Hit.GetActor());
		if (HitActor != nullptr && HitActor != this)
		{

			//BALOG(Warning, TEXT("Hit ABAEnemyCharacter Name : %s"), *Hit.Actor->GetName());
			UE_LOG(LogTemp, Log, TEXT("Hit Actor Name : %s, ServerUniqueID:%d"), *HitActor->GetName(), HitActor->GetServerUniqueID());

			
			bClickEnemy = true;
			ClickDestTarget = HitActor;
			ClickDestLocation = FVector::ZeroVector; // Enemy클릭시에는 사용안함

			//요놈은 몬스터에 써먹어야 될듯(몬스터가 ServerUniqueID가 없다면)
			//ServerSetEnemyTarget(HitActor->GetName());
			ServerSetTargetEnemyPlayer(HitActor->GetServerUniqueID());
			return;
		}
		else
		{
			bClickEnemy = false;
			ClickDestTarget = nullptr;
			ClickDestLocation = FVector::ZeroVector; // Enemy클릭시에는 사용안함
		}


		if (Hit.GetActor()->GetName().Equals(TEXT("Floor")))
		{
			

			//공격 모션 끝나고 이동을 할때 필요하다.
			ClickDestLocation = Hit.ImpactPoint;
			bClickEnemy = false;
			ClickDestTarget = nullptr;

			ServerCreatePathGuide(Hit.ImpactPoint, nullptr);
			//ServerClickLandscape();// release target on server.
			UE_LOG(LogTemp, Log, TEXT("Hit Actor Name : %s"), *Hit.GetActor()->GetName());

		}

	}
}

bool ALKCharacter::ServerCreatePathGuide_Validate(const FVector DestLocation, ACharacter* DestTarget)
{
	return true;
}

void ALKCharacter::ServerCreatePathGuide_Implementation(const FVector DestLocation, ACharacter* DestTarget)
{
	UE_LOG(LogTemp, Log, TEXT("ServerCreatePathGuide_Implementation"));

	FVector Dir = DestLocation - GetActorLocation();
	if (nullptr == PathGuide)
	{

		PathGuide = GetWorld()->SpawnActor<ALKPathGuide>(PathGuideClass, GetActorLocation(), GetActorRotation());

	}

	if (PathGuide)
	{
		if (GetPlayerState() != nullptr)
		{
			PathGuide->OwnerPlayerId = GetPlayerState()->GetPlayerId();// for client character following
			PathGuide->OwnerPCName = GetController()->GetName(); // for terminating when player logout
		}

		float Distance = FVector::Dist(PathGuide->GetActorLocation(), GetActorLocation());

		if (GetLocalRole() == ROLE_Authority && (GetVelocity() == FVector::ZeroVector || Distance > 100.0f))
		{
			if (PathGuide)
				PathGuide->SetActorLocation(GetActorLocation());

			
		}

		PathGuide->SetDestTarget(DestTarget);
		PathGuide->SetDestination(DestLocation);
	}

	ClientFollowPathGuide(true);
}




void ALKCharacter::FollowPathGuideProcess()
{
	if (!IsLocallyControlled()) return;
	if (IsAttacking) return; //공격 중이면

	if (bFollowPathGuide)
	{
		if (nullptr == PathGuideClientPtr)
		{			

			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), PathGuideClass, FoundActors);
			for (int i = 0; i < FoundActors.Num(); ++i)
			{
				auto Temp = Cast<ALKPathGuide>(FoundActors[i]);

				if (nullptr != Temp)
				{
					int32 OwnerPlayerId = Temp->OwnerPlayerId;
					int32 MyPlayerId = GetPlayerState()->GetPlayerId();
					if (MyPlayerId == OwnerPlayerId)
					{
						UE_LOG(LogTemp, Log, TEXT("PathGuide OwnerPlayerId : %d, My PlayerId : %d"), OwnerPlayerId, MyPlayerId);
						//BALOG(Warning, TEXT("PathGuide OwnerPlayerId : %d, My PlayerId : %d"), OwnerPlayerId, MyPlayerId);
						PathGuideClientPtr = Temp;
						break;
					}
				}
			}
		}

		if (PathGuideClientPtr)
		{			

			//공격모션끝나고 쫓아 갈려고 할때는 PathGuide가 너무 멀리 간 상태
			//다시 SetDestination하는 게 좋다.
			//클릭할경우 저장되는 변수 :bClickEnemy,ClickDestTarget,ClickDestLocation
			float Distance = FVector::Dist(PathGuideClientPtr->GetActorLocation(), GetActorLocation());
			if (Distance > 100)
			{
				//아래 ServerSetTargetEnemyPlayer 혹은 ServerCreatePathGuide  호출되고 나면 ClientFollowPathGuide(true); 를 통해서 true가 다시 되면서
				//다시 따라가게 된다. 여기서 false를 안하면 Tick에 의해 ServerSetTargetEnemyPlayer 혹은 ServerCreatePathGuide 호출이  여러번 일어나게 된다.
				bFollowPathGuide = false;

				if (bClickEnemy)
				{
					//ServerSetEnemyTarget(ClickDestTarget->GetName());
					ServerSetTargetEnemyPlayer(ClickDestTarget->GetServerUniqueID());
				}
				else
				{
					ServerCreatePathGuide(ClickDestLocation, nullptr);
				}

				return;
			}


			Distance = FVector::Dist(PathGuideClientPtr->GetActorLocation(), GetActorLocation());
			if (Distance >= 20.0f)
			{
				FVector Direction = PathGuideClientPtr->GetActorLocation() - GetActorLocation();
				AddMovementInput(Direction.GetSafeNormal() * 1.0f);

			}
		}
	}

}

void ALKCharacter::ClientDrawDebugCapsule_Implementation(bool NewResult, float NewAttackRange, float NewAttackRadius)
{
#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * NewAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = NewAttackRange * 0.5f + NewAttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = NewResult ? FColor::Green : FColor::Red;
	
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		NewAttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
}

void ALKCharacter::ClientFollowPathGuide_Implementation(bool bFollow)
{
	bFollowPathGuide = bFollow;
}


bool ALKCharacter::ServerDestroyPetAsset_Validate()
{
	return true;
}

void ALKCharacter::ServerDestroyPetAsset_Implementation()
{
	if (nullptr != CurPet)
	{
		CurPet->Destroy();
		CurPet = nullptr;
	}
}

bool ALKCharacter::ServerCreatePetAsset_Validate(int32 NewAsset)
{
	return true;
}

void ALKCharacter::ServerCreatePetAsset_Implementation(int32 NewAsset)
{
	
	if (nullptr == CurPet)
	{
		FVector SpawnLocation = GetActorLocation() + (GetActorRightVector() * 50.0f);
		CurPet = GetWorld()->SpawnActor<ALKPet>(PetClass, SpawnLocation, GetActorRotation());
		
	}

	if (CurPet)
	{
		if (GetPlayerState() != nullptr)
		{
			CurPet->OwnerPlayerId = GetPlayerState()->GetPlayerId();// for client character following
			CurPet->OwnerPCName = GetController()->GetName(); // for terminating when player logout
		}

		CurPet->SetDestTarget(this);


		auto DefaultSetting = GetDefault<ULKCharacterSetting>();
		int32 Rand = FMath::FRandRange(0, DefaultSetting->PetAssets.Num() - 1);
		CurPet->SetAssetNum(Rand);
	
	}

}

bool ALKCharacter::ServerChangeAvatarType_Validate(int32 NewType)
{
	return true;
}

void ALKCharacter::ServerChangeAvatarType_Implementation(int32 NewType)
{
	CurrentAvatarType = NewType;	
	OnRep_AvatarType();
}

bool ALKCharacter::ServerDance_Validate(bool NewDance)
{
	return true;
}

void ALKCharacter::ServerDance_Implementation(bool NewDance)
{
	bDance = NewDance;
}


bool ALKCharacter::ServerChangeCharacterName_Validate(const FString& NewName)
{
	return true;
}

void ALKCharacter::ServerChangeCharacterName_Implementation(const FString& NewName)
{
	//const FString CharacterFullName = FString::Printf(TEXT("%s%02d"), *NewName, ServerUniqueID);

	//CharacterName = CharacterFullName;
	CharacterName = NewName;
OnRep_CharacterName();// for host
}


bool ALKCharacter::ServerSetEnemyTarget_Validate(const FString& NewActorName)
{
	return true;
}

//@todo : 클릭을 할때 모든 액터를 돌면서 찾는것은 퍼포먼스에 좋지 않다.
// 최적화 필요.
void ALKCharacter::ServerSetTargetEnemyPlayer_Implementation(int32 NewServerUniqueID)
{
	for (TActorIterator<ALKCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (NewServerUniqueID == ActorItr->GetServerUniqueID())
		{

			TargetEnemy = Cast<ALKCharacter>(*ActorItr);
			ServerCreatePathGuide(ActorItr->GetActorLocation(), TargetEnemy);



			//// avoid to add multiple times
			//TargetEnemy->OnHPChanged.RemoveAll(this);
			//TargetEnemy->OnHPChanged.AddUObject(this, &ABACharacter::OnTargetHPChanged);

			//TargetEnemy->OnDead.RemoveAll(this);
			//TargetEnemy->OnDead.AddUObject(this, &ABACharacter::OnTargetDead);

			//ClientShowUITargetInfo(TargetEnemy->GetDisplayName(), TargetEnemy->GetHPRatio());
			break;
		}
	}
}

bool ALKCharacter::ServerSetTargetEnemyPlayer_Validate(int32 NewServerUniqueID)
{
	return true;
}

void ALKCharacter::ServerSetEnemyTarget_Implementation(const FString& NewActorName)
{
	for (TActorIterator<ALKCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (NewActorName.Equals(ActorItr->GetName()))
		{

			TargetEnemy = Cast<ALKCharacter>(*ActorItr);
			ServerCreatePathGuide(ActorItr->GetActorLocation(), TargetEnemy);



			//// avoid to add multiple times
			//TargetEnemy->OnHPChanged.RemoveAll(this);
			//TargetEnemy->OnHPChanged.AddUObject(this, &ABACharacter::OnTargetHPChanged);

			//TargetEnemy->OnDead.RemoveAll(this);
			//TargetEnemy->OnDead.AddUObject(this, &ABACharacter::OnTargetDead);

			//ClientShowUITargetInfo(TargetEnemy->GetDisplayName(), TargetEnemy->GetHPRatio());
			break;
		}
	}

}



//bool ALKCharacter::ServerChangePartsHairAsset_Validate()
//{
//	return true;
//}
//
//void ALKCharacter::ServerChangePartsHairAsset_Implementation()
//{
//	int32 RandIndex_Hair = -1;
//	if (CurrentAvatarType == (int32)EAvatarType::FEMALE_CITIZEN)
//	{
//		auto DefaultSetting = GetDefault<ULKCharacterSetting>();
//
//		RandIndex_Hair = FMath::FRandRange(0, DefaultSetting->PartsHair.Num());
//
//
//		if (-1 != RandIndex_Hair)
//		{
//			PartsHairID = RandIndex_Hair;
//			OnRep_PartsHairID(); //for host
//		}
//	}
//}

bool ALKCharacter::ServerChangePartsAsset_Validate(int32 NewEPartsType)
{
	return true;
}

void ALKCharacter::ServerChangePartsAsset_Implementation(int32 NewEPartsType)
{
	//Check if Current avatar typs has Parts
	if (!IsPartsAvatar()) return;
	
	auto DefaultSetting = GetDefault<ULKCharacterSetting>();
	int32 RandIndex_Hair = -1;
	int32 RandIndex_Top = -1;
	int32 RandIndex_Pants = -1;
	int32 RandIndex_Onepiece = -1;
	
		
	switch (NewEPartsType)
	{
		case (int32)EPartsType::PARTS_HAIR:
			if (CurrentAvatarType == (int32)EAvatarType::FEMALE_CITIZEN)
				RandIndex_Hair = FMath::FRandRange(0, DefaultSetting->PartsHair.Num());
			else if(CurrentAvatarType == (int32)EAvatarType::MALE_CITIZEN)
				RandIndex_Hair = FMath::FRandRange(0, DefaultSetting->PartsHair_Male.Num());

			if (-1 != RandIndex_Hair)
			{
				PartsHairID = RandIndex_Hair;
				OnRep_PartsHairID(); //for host
			}
			
			
		break;
		case (int32)EPartsType::PARTS_TOP:
			if (CurrentAvatarType == (int32)EAvatarType::FEMALE_CITIZEN)
				RandIndex_Top = FMath::FRandRange(0, DefaultSetting->PartsTop.Num());
			else if (CurrentAvatarType == (int32)EAvatarType::MALE_CITIZEN)
				RandIndex_Top = FMath::FRandRange(0, DefaultSetting->PartsTop_Male.Num());

			if (-1 != PartsTopID)
			{
				PartsTopID = RandIndex_Top;
				OnRep_PartsTopID(); //for host
			}
			
						
		break;
		case (int32)EPartsType::PARTS_PANTS:
			if (CurrentAvatarType == (int32)EAvatarType::FEMALE_CITIZEN)
				RandIndex_Pants = FMath::FRandRange(0, DefaultSetting->PartsPants.Num());
			else if (CurrentAvatarType == (int32)EAvatarType::MALE_CITIZEN)
				RandIndex_Pants = FMath::FRandRange(0, DefaultSetting->PartsPants_Male.Num());

			if (-1 != PartsPantsID)
			{
				PartsPantsID = RandIndex_Pants;
				OnRep_PartsPantsID(); //for host
			}
			
			
		break;
		case (int32)EPartsType::PARTS_ONEPIECE:
			if (CurrentAvatarType == (int32)EAvatarType::FEMALE_CITIZEN)
				RandIndex_Onepiece = FMath::FRandRange(0, DefaultSetting->PartsOnepiece.Num());
			else if (CurrentAvatarType == (int32)EAvatarType::MALE_CITIZEN)
				RandIndex_Onepiece = -1; //Male Citizen 은 Onepiece가 없다
			


			if (-1 != PartsOnepieceID)
			{
				PartsOnepieceID = RandIndex_Onepiece;
				OnRep_PartsOnepieceID(); //for host
			}

			



			break;
	}
	
		
	
	
}

bool ALKCharacter::ServerInitCharacterAsset_Validate()
{
	return true;
}

void ALKCharacter::ServerInitCharacterAsset_Implementation()
{
	//@todo : 플레이어의 에셋정보(from DB) 읽어와서 설정해야 함.
	auto DefaultSetting = GetDefault<ULKCharacterSetting>();
	int32 RandIndex = -1;// FMath::FRandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	

	switch (CurrentAvatarType)
	{	
	case (int32)EAvatarType::MALE_DEFAULT:
		RandIndex = FMath::FRandRange(0, DefaultSetting->CharacterAssets_Male.Num());
		break;
	case (int32)EAvatarType::FEMALE_DEFAULT:
		RandIndex = FMath::FRandRange(0, DefaultSetting->CharacterAssets_Female.Num());
		break;
	case (int32)EAvatarType::FEMALE_SCHOOLGIRL:
		RandIndex = FMath::FRandRange(0, DefaultSetting->CharacterAssets_Female_SchoolGirl.Num());
		break;
	case (int32)EAvatarType::MALE_CITIZEN:
		RandIndex = FMath::FRandRange(0, DefaultSetting->CharacterAssets_Male_Citizen.Num());
	case (int32)EAvatarType::FEMALE_CITIZEN:
		RandIndex = FMath::FRandRange(0, DefaultSetting->CharacterAssets_Female_Citizen.Num());
		
	}
	

	if (-1 != RandIndex)
	{
		CharacterAssetID = RandIndex;
		OnRep_CharacterAsset();//for host
	}

	
	

	//무기를 사용하지 않기 때문에 아래 주석 처리 함. 사용할 경우 아래 주석 제거
	//RandIndex = FMath::FRandRange(0, DefaultSetting->WeaponAssets.Num() - 1);
	//WeaponAssetID = RandIndex;
	//OnRep_WeaponAsset()//for host

	//펫 스폰 테스트
	//RandIndex = FMath::FRandRange(0, DefaultSetting->PetAssets.Num() - 1);
	//PetAssetNum = RandIndex;
	//OnRep_PetAsset();//for host
	 
}



bool ALKCharacter::ServerChangeDanceID_Validate(int32 NewDanceID)
{
	return true;
}

void ALKCharacter::ServerChangeDanceID_Implementation(int32 NewDanceID)
{
	DanceID = NewDanceID;
}

void ALKCharacter::OnRep_Dead()
{
	if (bDied)
	{
		LKAnim->SetDeadAnim();
		//SetActorEnableCollision(false);
	}
	
}

void ALKCharacter::OnRep_CharacterAsset()
{
	if (CharacterAssetID == -1) return;	

	//LKLOG(Warning, TEXT("OnRep_CharacterAsset"));
	auto DefaultSetting = GetDefault<ULKCharacterSetting>();
	

	switch (CurrentAvatarType)
	{
	case (int32)EAvatarType::FEMALE_DEFAULT:
		//1개 이상
		if (DefaultSetting->CharacterAssets_Female.Num() > 0)
		{
			//에셋의 수보다 CharacterAssetID가 작아야 한다. 
			if (CharacterAssetID < DefaultSetting->CharacterAssets_Female.Num())
			{
				CharacterAssetToLoad = DefaultSetting->CharacterAssets_Female[CharacterAssetID];
			}
			else
			{
				//기본 에셋으로 렌더링
				CharacterAssetToLoad = DefaultSetting->CharacterAssets_Female[0];
			}
		}		

		
		break;
	case (int32)EAvatarType::MALE_DEFAULT:
		//1개 이상
		if (DefaultSetting->CharacterAssets_Male.Num() > 0)
		{
			//에셋의 수보다 CharacterAssetID가 작아야 한다. 
			if (CharacterAssetID < DefaultSetting->CharacterAssets_Male.Num())
			{
				CharacterAssetToLoad = DefaultSetting->CharacterAssets_Male[CharacterAssetID];
			}
			else
			{
				//기본 에셋으로 렌더링
				CharacterAssetToLoad = DefaultSetting->CharacterAssets_Male[0];
			}
		}

		
		break;
	case (int32)EAvatarType::FEMALE_SCHOOLGIRL:
		//1개 이상
		if (DefaultSetting->CharacterAssets_Female_SchoolGirl.Num() > 0)
		{
			//에셋의 수보다 CharacterAssetID가 작아야 한다. 
			if (CharacterAssetID < DefaultSetting->CharacterAssets_Female_SchoolGirl.Num())
			{
				CharacterAssetToLoad = DefaultSetting->CharacterAssets_Female_SchoolGirl[CharacterAssetID];
			}
			else
			{
				//기본 에셋으로 렌더링
				CharacterAssetToLoad = DefaultSetting->CharacterAssets_Female_SchoolGirl[0];
			}
		}

		break;
	
	case (int32)EAvatarType::MALE_CITIZEN: //파츠

		//1개 이상
		if (DefaultSetting->CharacterAssets_Male_Citizen.Num() > 0)
		{
			//에셋의 수보다 CharacterAssetID가 작아야 한다. 
			if (CharacterAssetID < DefaultSetting->CharacterAssets_Male_Citizen.Num())
			{
				CharacterAssetToLoad = DefaultSetting->CharacterAssets_Male_Citizen[CharacterAssetID];
			}
			else
			{
				//기본 에셋으로 렌더링
				CharacterAssetToLoad = DefaultSetting->CharacterAssets_Male_Citizen[0];
			}
		}

		//파츠 Init		
		PartsHairToLoad = DefaultSetting->PartsHair_Male[0];
		PartsTopToLoad = DefaultSetting->PartsTop_Male[0];
		PartsPantsToLoad = DefaultSetting->PartsPants_Male[0];
		PartsShoesToLoad = DefaultSetting->PartsShoes_Male[0];
		
		//@todo: Onepiece 를 입은 상태인지
		Onepiece->SetSkeletalMesh(nullptr);
		break;

	case (int32)EAvatarType::FEMALE_CITIZEN: //파츠
	
		//1개 이상
		if (DefaultSetting->CharacterAssets_Female_Citizen.Num() > 0)
		{
			//에셋의 수보다 CharacterAssetID가 작아야 한다. 
			if (CharacterAssetID < DefaultSetting->CharacterAssets_Female_Citizen.Num())
			{
				CharacterAssetToLoad = DefaultSetting->CharacterAssets_Female_Citizen[CharacterAssetID];
			}
			else
			{
				//기본 에셋으로 렌더링
				CharacterAssetToLoad = DefaultSetting->CharacterAssets_Female_Citizen[0];
			}
		}
		//파츠 Init		//@todo : 저장된 partsID (replicated) 을 이용할것
		PartsHairToLoad = DefaultSetting->PartsHair[0];
		PartsTopToLoad = DefaultSetting->PartsTop[0];
		PartsPantsToLoad = DefaultSetting->PartsPants[0];
		PartsShoesToLoad = DefaultSetting->PartsShoes[0];
		
		//@todo: Onepiece 를 입은 상태인지
		Onepiece->SetSkeletalMesh(nullptr);
		break;
	}


	auto LKGameInstance = Cast<ULKGameInstance>(GetGameInstance());
	if (nullptr != LKGameInstance)
	{
		AssetStreamingHandle = LKGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &ALKCharacter::OnAssetLoadCompleted));
		if (IsPartsAvatar())
		{
			AssetStreamingHandle_PartsHair = LKGameInstance->StreamableManager.RequestAsyncLoad(PartsHairToLoad, FStreamableDelegate::CreateUObject(this, &ALKCharacter::OnAssetLoadCompleted_PartsHair));
			AssetStreamingHandle_PartsTop = LKGameInstance->StreamableManager.RequestAsyncLoad(PartsTopToLoad, FStreamableDelegate::CreateUObject(this, &ALKCharacter::OnAssetLoadCompleted_PartsTop));
			AssetStreamingHandle_PartsPants = LKGameInstance->StreamableManager.RequestAsyncLoad(PartsPantsToLoad, FStreamableDelegate::CreateUObject(this, &ALKCharacter::OnAssetLoadCompleted_PartsPants));
			AssetStreamingHandle_PartsShoes = LKGameInstance->StreamableManager.RequestAsyncLoad(PartsShoesToLoad, FStreamableDelegate::CreateUObject(this, &ALKCharacter::OnAssetLoadCompleted_PartsShoes));
		}
		
	}
}


bool ALKCharacter::IsPartsAvatar()
{
	if (CurrentAvatarType == (int32)EAvatarType::MALE_CITIZEN || CurrentAvatarType == (int32)EAvatarType::FEMALE_CITIZEN)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ALKCharacter::OnRep_PartsOnepieceID()
{
	if (PartsOnepieceID == -1) return;

	auto DefaultSetting = GetDefault<ULKCharacterSetting>();

	if (CurrentAvatarType == (int32)EAvatarType::FEMALE_CITIZEN)
	{

		if (DefaultSetting->PartsOnepiece.Num() > 0)
		{
			if (PartsOnepieceID < DefaultSetting->PartsOnepiece.Num())
			{
				PartsOnepieceToLoad = DefaultSetting->PartsOnepiece[PartsOnepieceID];
			}
			else
			{
				//기본 에셋으로 렌더링
				PartsOnepieceToLoad = DefaultSetting->PartsOnepiece[0];
			}
		}
	}
	else
	{
		return; //@todo Male 은 Onepiece 가 아직 없음
	}

	auto LKGameInstance = Cast<ULKGameInstance>(GetGameInstance());
	if (nullptr != LKGameInstance)
	{

		AssetStreamingHandle_PartsOnepiece = LKGameInstance->StreamableManager.RequestAsyncLoad(PartsOnepieceToLoad, FStreamableDelegate::CreateUObject(this, &ALKCharacter::OnAssetLoadCompleted_PartsOnepiece));

	}
}

void ALKCharacter::OnRep_PartsPantsID()
{
	if (PartsPantsID == -1) return;
	//통짜 아바타는 파츠 변경이 안된다.
	if (!IsPartsAvatar()) return;
	auto DefaultSetting = GetDefault<ULKCharacterSetting>();
	if (CurrentAvatarType == (int32)EAvatarType::MALE_CITIZEN)
	{
		if (DefaultSetting->PartsPants_Male.Num() > 0)
		{
			if (PartsPantsID < DefaultSetting->PartsPants_Male.Num())
			{
				PartsPantsToLoad = DefaultSetting->PartsPants_Male[PartsPantsID];
			}
			else
			{
				//기본 에셋으로 렌더링
				PartsPantsToLoad = DefaultSetting->PartsPants_Male[0];
			}
		}
	}
	else if (CurrentAvatarType == (int32)EAvatarType::FEMALE_CITIZEN)
	{
		if (DefaultSetting->PartsPants.Num() > 0)
		{
			if (PartsPantsID < DefaultSetting->PartsPants.Num())
			{
				PartsPantsToLoad = DefaultSetting->PartsPants[PartsPantsID];
			}
			else
			{
				//기본 에셋으로 렌더링
				PartsPantsToLoad = DefaultSetting->PartsPants[0];
			}
		}
	}
	else
	{
		return;
	}

	auto LKGameInstance = Cast<ULKGameInstance>(GetGameInstance());
	if (nullptr != LKGameInstance)
	{

		AssetStreamingHandle_PartsPants = LKGameInstance->StreamableManager.RequestAsyncLoad(PartsPantsToLoad, FStreamableDelegate::CreateUObject(this, &ALKCharacter::OnAssetLoadCompleted_PartsPants));

	}
}

void ALKCharacter::OnRep_PartsTopID()
{
	if (PartsTopID == -1) return;
	//통짜 아바타는 파츠 변경이 안된다.
	if (!IsPartsAvatar()) return;

	auto DefaultSetting = GetDefault<ULKCharacterSetting>();

	if (CurrentAvatarType == (int32)EAvatarType::MALE_CITIZEN)
	{

		if (DefaultSetting->PartsTop_Male.Num() > 0)
		{
			if (PartsTopID < DefaultSetting->PartsTop_Male.Num())
			{
				PartsTopToLoad = DefaultSetting->PartsTop_Male[PartsTopID];
			}
			else
			{
				//기본 에셋으로 렌더링
				PartsTopToLoad = DefaultSetting->PartsTop_Male[0];
			}
		}
	}
	else if (CurrentAvatarType == (int32)EAvatarType::FEMALE_CITIZEN)
	{
		if (DefaultSetting->PartsTop.Num() > 0)
		{
			if (PartsTopID < DefaultSetting->PartsTop.Num())
			{
				PartsTopToLoad = DefaultSetting->PartsTop[PartsTopID];
			}
			else
			{
				//기본 에셋으로 렌더링
				PartsTopToLoad = DefaultSetting->PartsTop[0];
			}
		}
	}
	else
	{
		return;
	}

	auto LKGameInstance = Cast<ULKGameInstance>(GetGameInstance());
	if (nullptr != LKGameInstance)
	{

		AssetStreamingHandle_PartsTop = LKGameInstance->StreamableManager.RequestAsyncLoad(PartsTopToLoad, FStreamableDelegate::CreateUObject(this, &ALKCharacter::OnAssetLoadCompleted_PartsTop));

	}
}

void ALKCharacter::OnRep_PartsHairID()
{
	if (PartsHairID == -1) return;

	//통짜 아바타는 파츠 변경이 안된다.
	if (!IsPartsAvatar()) return;

	auto DefaultSetting = GetDefault<ULKCharacterSetting>();

	if (CurrentAvatarType == (int32)EAvatarType::MALE_CITIZEN)
	{
		if (DefaultSetting->PartsHair_Male.Num() > 0)
		{
			if (PartsHairID < DefaultSetting->PartsHair_Male.Num())
			{
				PartsHairToLoad = DefaultSetting->PartsHair_Male[PartsHairID];
			}
			else
			{
				//기본 에셋으로 렌더링
				PartsHairToLoad = DefaultSetting->PartsHair_Male[0];
			}
		}
	}
	else if (CurrentAvatarType == (int32)EAvatarType::FEMALE_CITIZEN)
	{
		if (DefaultSetting->PartsHair.Num() > 0)
		{
			if (PartsHairID < DefaultSetting->PartsHair.Num())
			{
				PartsHairToLoad = DefaultSetting->PartsHair[PartsHairID];
			}
			else
			{
				//기본 에셋으로 렌더링
				PartsHairToLoad = DefaultSetting->PartsHair[0];
			}
		}
	}
	else
	{
		return;
	}
	
	

	auto LKGameInstance = Cast<ULKGameInstance>(GetGameInstance());
	if (nullptr != LKGameInstance)
	{
		
		AssetStreamingHandle_PartsHair = LKGameInstance->StreamableManager.RequestAsyncLoad(PartsHairToLoad, FStreamableDelegate::CreateUObject(this, &ALKCharacter::OnAssetLoadCompleted_PartsHair));

	}

}



void ALKCharacter::OnRep_WeaponAsset()
{
	
	if (WeaponAssetID == -1) return;

	if (nullptr != CurWeapon)
	{
		CurWeapon->Destroy();
	}

	FName WeaponSocket(TEXT("hand_rSocket"));
	CurWeapon = GetWorld()->SpawnActor<ALKWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->SetMesh(WeaponAssetID);
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}

}



void ALKCharacter::OnRep_CharacterName()
{
	IsCharacterNameReplicated = true;
	UpdateCharacterNameWidget();
	/*auto CharacterWidget = Cast<ULKCharacterWidget>(CharacterNameWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->SetCharacterName(CharacterName);
	}*/
}

void ALKCharacter::OnRep_AvatarType()
{
	int i = 0;
	UE_LOG(LogTemp, Log, TEXT("OnRep_AvatarType"));
	OnRep_CharacterAsset();
}

//void ALKCharacter::OnRep_DanceID()
//{
//	if (LKAnim)
//		LKAnim->SetDanceID(DanceID);
//}

void ALKCharacter::UpDown(float NewAxisValue)
{
	if (IsDead()) return;

	if (NewAxisValue != 0.0f)
	{
		bFollowPathGuide = false;
	}

	//공격모션 중에는 이동 못함
	if (IsAttacking) return;
	
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), (NewAxisValue * Speed));
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;
	}
}

void ALKCharacter::LeftRight(float NewAxisValue)
{
	if (IsDead()) return;

	if (NewAxisValue != 0.0f)
	{
		bFollowPathGuide = false;
	}
	
	//공격모션 중에는 이동 못함
	if (IsAttacking) return;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:		
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), (NewAxisValue * Speed));
		break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}

void ALKCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		if(bTurnView)
			AddControllerPitchInput(NewAxisValue);
		break;
	}
}

void ALKCharacter::Turn(float NewAxisValue)
{	
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		if(bTurnView)
			AddControllerYawInput(NewAxisValue);
		break;
	}
}

void ALKCharacter::JumpHook()
{
	if (IsDead()) return;

	bFollowPathGuide = false;
	Jump();
	
}

//BindAction
void ALKCharacter::ChangeAvatarType()
{

	int32 avatarToChange = CurrentAvatarType + 1;

	if ((int32)EAvatarType::END <= avatarToChange)
	{
		avatarToChange = 0;
		
	}
	
	ServerChangeAvatarType(avatarToChange);
	


	
	
}

//BindAction
void ALKCharacter::Dance()
{
	ServerDance(!bDance);
}

void ALKCharacter::ViewChange()
{
	//이 프로젝트에서는 ViewChange 안함
	/*switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		SetControlMode(EControlMode::GTA);
		break;
	}*/
}

void ALKCharacter::Attack()
{
	//(임시) 무기가 없을경우 공격하지 않는다
	if (nullptr == CurWeapon)
	{
		return;
	}


	if (IsDead()) return;

	if (IsAttacking)
	{
		LKCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
			
			UE_LOG(LogTemp, Warning, TEXT("[Client] IsComboInputOn = true, currentCombo:%d"), CurrentCombo);
		}
		
	}
	else
	{
		AttackStartComboState();
		if (LKAnim)
		{
			LKAnim->PlayAttackMontage();
			LKAnim->JumpToAttackMontageSection(CurrentCombo);
		}

		
		IsAttacking = true;

		bFollowPathGuide = false;
	}

	


	
}

void ALKCharacter::ServerAttack_Implementation(int32 NewCurrentCombo)
{
	if (IsDead()) return;

	CurrentCombo = NewCurrentCombo;	
		
	//LKLOG(Warning, TEXT("[RORL_Authority] ServerAttack_Implementation %d"), NewCurrentCombo);

}

bool ALKCharacter::ServerAttack_Validate(int32 NewCurrentCombo)
{
	return true;
}

void ALKCharacter::AttackCheck()
{
	if (IsDead()) return;

	if (IsLocallyControlled())
	{
		//UE_LOG(LogTemp, Warning, TEXT("[IsLocallyControlled] AttackCheck"));
		ServerHitCheck();

	}
}

void ALKCharacter::ServerHitCheck_Implementation()
{
	if (IsDead()) return;

	if (ROLE_Authority == GetLocalRole())
	{
		UE_LOG(LogTemp, Warning, TEXT("[RORL_Authority] ServerHitCheck_Implementation"));
		
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);
		bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation() + GetActorForwardVector() * AttackRange,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel3,
			FCollisionShape::MakeSphere(AttackRadius),
			Params);


#if ENABLE_DRAW_DEBUG
		ClientDrawDebugCapsule(bResult, AttackRange, AttackRadius);
#endif
		if (bResult)
		{
			if (HitResult.GetActor())
			{
				LKLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName());

				FDamageEvent DamageEvent;
				HitResult.GetActor()->TakeDamage(50.0f, DamageEvent, GetController(),this);
			}
		}
	}
}

bool ALKCharacter::ServerHitCheck_Validate()
{
	return true;
}

void ALKCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LKAnim = Cast<ULKAnimInstance>(GetMesh()->GetAnimInstance());
	
	if(LKAnim)
		LKAnim->OnMontageEnded.AddDynamic(this, &ALKCharacter::OnAttackMontageEnded);
	
	if (LKAnim)
		LKAnim->OnAttackHitCheck.AddUObject(this, &ALKCharacter::AttackCheck);
	

	if (LKAnim)
	{
		LKAnim->OnNextAttackCheck.AddLambda([this]() -> void {
			
			//Client만
			if (!IsRunningDedicatedServer())
			{
				
				CanNextCombo = false;

				if (IsComboInputOn)
				{

					AttackStartComboState();
					
					if (LKAnim)
						LKAnim->JumpToAttackMontageSection(CurrentCombo);

					

				}
			}
			
			
		});
	}
}



void ALKCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{	
	if (IsRunningDedicatedServer()) return;


	if (IsLocallyControlled())
	{
		LKCHECK(IsAttacking);
	}

	IsAttacking = false;
	AttackEndComboState();
	
}

//ReplicatedUsing
void ALKCharacter::OnCurrentCombo()
{
	//LKLOG(Warning, TEXT("OnCurrentCombo %d"), CurrentCombo);
	if (CurrentCombo == 0) return;

	IsAttacking = true;
	LKAnim->PlayAttackMontage();
	LKAnim->JumpToAttackMontageSection(CurrentCombo);

	
}

bool ALKCharacter::ServerSetCollisionProfileName_Validate(const FString& NewCollisionProfileName)
{
	return true;
}

void ALKCharacter::ServerSetCollisionProfileName_Implementation(const FString & NewCollisionProfileName)
{
	GetCapsuleComponent()->SetCollisionProfileName(*NewCollisionProfileName);
}

void ALKCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALKCharacter, bDied)
	DOREPLIFETIME(ALKCharacter, CurrentCombo)
	DOREPLIFETIME(ALKCharacter, CharacterAssetID)
	DOREPLIFETIME(ALKCharacter, PartsHairID)		
	DOREPLIFETIME(ALKCharacter, PartsTopID)
	DOREPLIFETIME(ALKCharacter, PartsPantsID)
	DOREPLIFETIME(ALKCharacter, PartsOnepieceID)		
	DOREPLIFETIME(ALKCharacter, WeaponAssetID)
	DOREPLIFETIME(ALKCharacter, ServerUniqueID)
	DOREPLIFETIME(ALKCharacter, CharacterName)
	DOREPLIFETIME(ALKCharacter, bDance)
	DOREPLIFETIME(ALKCharacter, DanceID)
	DOREPLIFETIME(ALKCharacter, CurrentAvatarType)
		

		
}
