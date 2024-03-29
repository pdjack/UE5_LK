# UE5_LK


1. Character를 상속받아 MyCharacter C++ 클래스 생성한다.


2. CharacterClass를 상속받아 블루프린트를 생성한다.


3. MyCharacter cpp 파일에서 SpringArm , Camera 생성 및 설정
```
public :
...
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
...
```


4. PlayerInput 구현 



캐릭터 Turn / LookUp 구현시 
![image](https://user-images.githubusercontent.com/29656900/182010656-3a4372af-5173-4462-92ed-c379186c8a0e.png)



![image](https://user-images.githubusercontent.com/29656900/183020027-46da7de2-02c1-4e5c-bf7e-bb1e82545192.png)


[Character Animation]

AnimInstance C++ class 추가 - MyAnimInstance

MyAnimInstance class 에 CurrentPawnSpeed 변수 추가
![image](https://user-images.githubusercontent.com/29656900/182016143-7e2c1154-df1f-4a8a-a945-f66ec659e942.png)


AnimBlueprint 생성 후 Class Settings 에서 ParentClass에서 C++ 클래스 선택
![image](https://user-images.githubusercontent.com/29656900/182018847-5ba6ef6a-760a-4ccb-9aab-eec4d05b2959.png)





애니메이션 블루프린트에서 변수 표시
![image](https://user-images.githubusercontent.com/29656900/182016114-3c516d81-8320-426f-a65f-d2fca44d9fc8.png)


State Machine 추가

![image](https://user-images.githubusercontent.com/29656900/182018988-8d8a242f-f3f4-48da-8494-7a7ec4ec6cb8.png)

![image](https://user-images.githubusercontent.com/29656900/182018999-b2e82a33-919c-442b-bcbb-62ed90ffd4ce.png)

Add State

![image](https://user-images.githubusercontent.com/29656900/182019054-f3320e03-b249-49d1-9025-ec1e311d58d3.png)


Ground State

![image](https://user-images.githubusercontent.com/29656900/182019074-d4223119-108a-4ea6-b677-ccbe5f70a459.png)


Add State (JumpStart, JumpLoop, JumpEnd)
![image](https://user-images.githubusercontent.com/29656900/182020836-d8274218-62e1-45cb-bfbd-2cafa66ea115.png)



Transition Rule : Ground => JumpStart

![image](https://user-images.githubusercontent.com/29656900/182020780-99cc715b-2662-4e12-92d2-a8fd1ff156fd.png)


Transition Rule :  JumpStart => JumpLoop 

![image](https://user-images.githubusercontent.com/29656900/183558600-5ffc7d05-6b98-49da-ab23-0e65aa39e895.png)









Montage 플레이 적용 하려면 AnimBP 에 Default Slot 추가해야 한다.

![image](https://user-images.githubusercontent.com/29656900/182529057-2fee1201-baef-45d3-a7fe-daebd3c6ccab.png)

![image](https://user-images.githubusercontent.com/29656900/182529148-105da419-8900-4d9d-8612-2c5b54e39538.png)


```
if(GEngine)
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Debug %f"), 123.0f));

if(GEngine)
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Debug")));
		
		
UE_LOG(LogTemp, Warning, TEXT("Hit ACtor Name : %s"), *HitResult.GetActor()->GetName());

```



[collision]

Add TraceChannel

![image](https://user-images.githubusercontent.com/29656900/182603030-0e8968d5-5cf4-4ab2-9871-ae354264edc3.png)

Config/DefaultEngine.ini

![image](https://user-images.githubusercontent.com/29656900/182603137-a6a790a0-767a-4328-b86c-1b27f82ecdf0.png)


```
void AMyCharacter::AttackHitCheck()
{
	float AttackRange = 200.0f;
	float AttackRadius = 50.0f;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("AMyCharacter::AttackHitCheck()"));

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange ,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;

	//Capsule 의 Z (Up)를 TraceVec 방향으로 회전 
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;

	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif

	if (bResult)
	{
		if (HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit ACtor Name : %s"), *HitResult.GetActor()->GetName());

			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(50.0f, DamageEvent, GetController(),this);
		}
	}

}
```


TakeDamage 받으면 Dead

![image](https://user-images.githubusercontent.com/29656900/182615041-c13245ec-e4db-4f42-a777-1b23c1c19922.png)


```
float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	UE_LOG(LogTemp, Warning, TEXT("ACtor : %s took Damage : %f"), *GetName(), FinalDamage);
	if (FinalDamage > 0.0f)
	{
		MyAnim->SetAnimDead();

		FVector Dir = DamageCauser->GetActorLocation() - GetActorLocation();
		Dir.Z = 0.0f;
		FQuat LookAtRot = FRotationMatrix::MakeFromX(Dir).ToQuat();
		SetActorRotation(LookAtRot);
	}
	return FinalDamage;
}
```


[Weapon]

MyCharacter.h 
```
public:
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
UStaticMeshComponent* Weapon;
	
```
MyCharacter.cpp
```
AMyCharacter::AMyCharacter()
{
...
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	//StaticMesh'/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Axe01SM.Axe01SM'
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WEAPON(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Axe01SM.Axe01SM"));
	if (SM_WEAPON.Succeeded())
	{
		Weapon->SetStaticMesh(SM_WEAPON.Object);
	}
...
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackHitCheck);
	
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{

		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
	}
}

```

