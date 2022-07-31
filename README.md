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


