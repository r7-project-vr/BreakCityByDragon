// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/VRDragon_ver1.h"
#include "InputMappingContext.h"
#include "FireBall/FireBall_ver2.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/ArrowComponent.h" 
#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputSubsystems.h"
// VR
#include "Engine/Engine.h"
#include "IXRTrackingSystem.h"
#include "HeadMountedDisplay.h"

// Sets default values
AVRDragon_ver1::AVRDragon_ver1():
	FB(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root;

	// エンハンス何とか
	{
		// Input Mapping Context「IMC_VRDragon」を読み込む
		DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/VRTemplate/Input/IMC_VRDragon"));

		// Input Action「IA_DragonFire」を読み込む
		ControlFire = LoadObject<UInputAction>(nullptr, TEXT("/Game/VRTemplate/Input/Actions/Dragon/IA_DragonFire"));
	}

	// Camera
	{
		// HMDの原点
		USceneComponent* CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
		CameraRoot->SetupAttachment(RootComponent);

		// Cameraを追加する
		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
		Camera->SetupAttachment(CameraRoot);
	}

	// FireBall
	{
		static ConstructorHelpers::FClassFinder<AActor> BPClass(TEXT("/Game/Level/Han/BP_MyFireBall_ver2"));
		if (BPClass.Succeeded())
		{
			BlueprintFireBall = BPClass.Class;
		}
	}
}

// Called when the game starts or when spawned
void AVRDragon_ver1::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 5);
		}
	}


	// VR
	if (GEngine && GEngine->XRSystem.IsValid())
	{
		bool VRAllowed = GEngine->XRSystem->IsHeadTrackingAllowed();

		if (VRAllowed) {

			// 顔面の高さに合わせる
			GEngine->XRSystem->SetTrackingOrigin(EHMDTrackingOrigin::Local);
		}
	}

}

// Called every frame
void AVRDragon_ver1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AVRDragon_ver1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// ControlBallとIA_ControlのTriggeredをBindする
		EnhancedInputComponent->BindAction(ControlFire, ETriggerEvent::Triggered, this, &AVRDragon_ver1::GoFire);
	}
}

// 火球コントロール
void AVRDragon_ver1::GoFire(const FInputActionValue& Value) {

	if (const bool B = Value.Get<bool>()) {

		//FRotator look = GetControlRotation();
		//look = Camera->GetComponentToWorld().GetRotation().Rotator();
		//FVector pos = GetActorLocation() + GetActorForwardVector() * 160;

		//FActorSpawnParameters SpawnParams;
		//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//if(!FB.IsValid())
		//FB = GetWorld()->SpawnActor<AFireBall_ver2>(BlueprintFireBall, pos, look); // スポーン処理 
	}
}
