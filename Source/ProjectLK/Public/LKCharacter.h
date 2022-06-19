// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectLK.h"
#include "GameFramework/Character.h"
#include "LKCharacter.generated.h"

UENUM()
enum class EAvatarType : uint8
{
	MALE_DEFAULT,
	FEMALE_DEFAULT,
	FEMALE_SCHOOLGIRL,
	MALE_CITIZEN,
	FEMALE_CITIZEN,
	END
	
};




UCLASS()
class PROJECTLK_API ALKCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALKCharacter();

	bool IsDead() { return bDied; }
	void SetServerUniqueID(uint32 uID);
	int32 GetServerUniqueID() { return ServerUniqueID; }

	bool IsDancing() { return bDance; }
	FString& GetCharacterName() { return CharacterName; }

	UFUNCTION(BlueprintCallable)
	void ChangeCharacterName(FString NewCharacterName);


	//데모만들때만 필요한 기능, 상용화땐 지원도 무방
	UFUNCTION(BlueprintCallable)
	void SetPetScale3D(float size);

	int32 GeDanceID() { return DanceID; }
	class ALKPet* GetCurPet() { return CurPet; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DIABLO
	};

	enum class EPartsType
	{
		PARTS_DEFAULT,
		PARTS_HAIR,
		PARTS_TOP,
		PARTS_PANTS,
		PARTS_SHOES,
		PARTS_ONEPIECE
	};
	void SetControlMode(EControlMode ControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	
	
	


	TSubclassOf<UAnimInstance> MALE_ANIM_CLASS;
	TSubclassOf<UAnimInstance> FEMALE_ANIM_CLASS;
	TSubclassOf<UAnimInstance> MALE_CITIZEN_ANIM_CLASS;
	TSubclassOf<UAnimInstance> FEMALE_CITIZEN_ANIM_CLASS;
	TSubclassOf<UAnimInstance> FEMALE_SCHOOLGIRL_ANIM_CLASS;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class ULKCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* CharacterNameWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Parts)
	USkeletalMeshComponent* Hair;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Parts)
	USkeletalMeshComponent* Top;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Parts)
	USkeletalMeshComponent* Pants;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Parts)
	USkeletalMeshComponent* Shoes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Parts)
	USkeletalMeshComponent* Onepiece;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	float Speed;

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttack(int32 NewCurrentCombo);

	
	bool IsAttacking = false;

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerCreatePetAsset(int32 NewAsset);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDestroyPetAsset();


	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerChangeAvatarType(int32 NewAvatarType);
	//void ServerChangeAvatarType(TEnumAsByte<EAvatarType> NewType);
	

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerDance(bool NewDance);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeCharacterName(const FString& NewName);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetEnemyTarget(const FString& NewActorName);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetTargetEnemyPlayer(int32 NewServerUniqueID);


	UFUNCTION(Client, Reliable)
	void ClientFollowPathGuide(bool bFollow);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCreatePathGuide(const FVector DestLocation, class ACharacter* DestTarget);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCollisionProfileName(const FString& NewCollisionProfileName);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHitCheck();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInitCharacterAsset();


	/*UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangePartsHairAsset();*/


	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerChangePartsAsset(int32 NewEPartsType);


	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerChangeDanceID(int32 NewDanceID);

	/**  class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = PathGuide)
	TSubclassOf<class ALKPathGuide> PathGuideClass;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class ALKWeapon> WeaponClass;
	
	UPROPERTY(EditDefaultsOnly, Category = Pet)
	TSubclassOf<class ALKPet> PetClass;

	class ALKPathGuide* PathGuideClientPtr = nullptr; //@todo weak ptr

	class ALKPathGuide* PathGuide = nullptr;

	bool bFollowPathGuide = false;

	class ALKPet* CurPet = nullptr;

	bool IsPartsAvatar();


	UPROPERTY(ReplicatedUsing = OnRep_Dead, BlueprintReadOnly, Category = "Player")
	bool bDied;
	
	UFUNCTION()
	void OnRep_Dead();


	UPROPERTY(ReplicatedUsing = OnRep_CharacterAsset, BlueprintReadOnly, Category = "Player")
	int32 CharacterAssetID;

	UFUNCTION()
	void OnRep_CharacterAsset();

	UPROPERTY(ReplicatedUsing = OnRep_PartsHairID, BlueprintReadOnly, Category = "Player")
	int32 PartsHairID;

	UFUNCTION()
	void OnRep_PartsHairID();



	UPROPERTY(ReplicatedUsing = OnRep_PartsTopID, BlueprintReadOnly, Category = "Player")
	int32 PartsTopID;

	UFUNCTION()
	void OnRep_PartsTopID();


	UPROPERTY(ReplicatedUsing = OnRep_PartsPantsID, BlueprintReadOnly, Category = "Player")
	int32 PartsPantsID;

	UFUNCTION()
	void OnRep_PartsPantsID();


	UPROPERTY(ReplicatedUsing = OnRep_PartsOnepieceID, BlueprintReadOnly, Category = "Player")
	int32 PartsOnepieceID;

	UFUNCTION()		
	void OnRep_PartsOnepieceID();


	UPROPERTY(ReplicatedUsing = OnRep_WeaponAsset, BlueprintReadOnly, Category = "Player")
	int32 WeaponAssetID;
	UFUNCTION()
	void OnRep_WeaponAsset();

	//UPROPERTY(ReplicatedUsing = OnRep_PetAsset, BlueprintReadOnly, Category = "Player")
	//int32 PetAssetNum;

	//UFUNCTION()
	//void OnRep_PetAsset();


	UPROPERTY(ReplicatedUsing = OnRep_CharacterName, BlueprintReadOnly, Category = "Player")
	FString CharacterName;

	UFUNCTION()
	void OnRep_CharacterName();



	
	//UPROPERTY(ReplicatedUsing = OnRep_AvatarType, BlueprintReadWrite, Category = "Player")
	//TEnumAsByte<EAvatarType> CurrentAvatarType;

	UFUNCTION()
	void OnRep_AvatarType();

	UPROPERTY(ReplicatedUsing = OnRep_AvatarType, BlueprintReadWrite, Category = "Player")
	int32 CurrentAvatarType = (int32)EAvatarType::FEMALE_CITIZEN;



	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	int32 ServerUniqueID;


	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bDance;

	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Player")
	int32 DanceID;

	//UFUNCTION()
	//void OnRep_DanceID();

	
	//마우스 클릭으로 이동 할때 저장해 놓는다.(Enemey,Floor)
	bool bClickEnemy = false;
	class ALKCharacter* ClickDestTarget = nullptr;
	FVector ClickDestLocation = FVector::ZeroVector;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void JumpHook();

	void ChangeAvatarType();

	void ViewChange();
	void TurnViewPressed();
	void TurnViewReleased();
	void InputClickPressed();
	void InputClickReleased();

	void ChangeCharacterAsset();
	void ChangeWeaponAsset();

	void CreatePetAsset();
	void DestroyPetAsset();

	void MoveToMouseCursor();
	void FollowPathGuideProcess();

	void Attack();

	void AttackCheck();

	void Dance();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();


	UFUNCTION(Client, Reliable)
	void ClientDrawDebugCapsule(bool NewResult, float NewAttackRange, float NewAttackRadius);

	void OnAssetLoadCompleted();
	void OnAssetLoadCompleted_PartsHair();
	void OnAssetLoadCompleted_PartsTop();
	void OnAssetLoadCompleted_PartsPants();
	void OnAssetLoadCompleted_PartsShoes();
	void OnAssetLoadCompleted_PartsOnepiece();

	void SetAnimInstance();

	void SetPartsAnimInstanceClass(UClass * NewClass);

	void SetPartsHiddenInGame(bool hidden);


	void UpdateCharacterNameWidget();
private:
	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	//bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(ReplicatedUsing = OnCurrentCombo, VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UFUNCTION()
	void OnCurrentCombo();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY()
	class ULKAnimInstance* LKAnim;

	bool bTurnView = false;

	bool bClickMouse = false;

	ALKCharacter * TargetEnemy = nullptr;

	
	class ALKWeapon* CurWeapon = nullptr;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	FSoftObjectPath PartsHairToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle_PartsHair;

	FSoftObjectPath PartsTopToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle_PartsTop;
	
	FSoftObjectPath PartsPantsToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle_PartsPants;


	FSoftObjectPath PartsShoesToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle_PartsShoes;

	FSoftObjectPath PartsOnepieceToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle_PartsOnepiece;


	bool IsAlreadySetCharacterName = false;
	bool IsCharacterNameReplicated = false;
};
