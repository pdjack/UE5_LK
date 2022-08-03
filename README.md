# UE5_LK


1. Character를 상속받아 MyCharacter C++ 클래스 생성한다.


2. CharacterClass를 상속받아 블루프린트를 생성한다.


3. MyCharacter cpp 파일에서 SpringArm , Camera 생성 및 설정

4. PlayerInput 구현 



캐릭터 Turn / LookUp 구현시 
![image](https://user-images.githubusercontent.com/29656900/182010656-3a4372af-5173-4462-92ed-c379186c8a0e.png)



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

![image](https://user-images.githubusercontent.com/29656900/182020897-98d72323-30a1-4c88-8b0c-db7f6840727e.png)









Montage 플레이 적용 하려면 AnimBP 에 Default Slot 추가해야 한다.

![image](https://user-images.githubusercontent.com/29656900/182529057-2fee1201-baef-45d3-a7fe-daebd3c6ccab.png)

![image](https://user-images.githubusercontent.com/29656900/182529148-105da419-8900-4d9d-8612-2c5b54e39538.png)


'''
if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Debug %f"), 123.0f));
