// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDragon/VRDragon_ver2.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/ArrowComponent.h" 
#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "FireBall/FireBall_ver1.h"
// VR
#include "Engine/Engine.h"
#include "IXRTrackingSystem.h"
#include "HeadMountedDisplay.h"

// Sets default values
AVRDragon_ver2::AVRDragon_ver2():
	FireChargeCnt(0),
	CanFire(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// StaticMeshComponentを追加し、RootComponentに設定する
	Player = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = Player;

	// SphereComponentを追加し、BoxComponentをRootComponentにAttachする
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Sphere->SetupAttachment(RootComponent);

	// Sphereのサイズを設定する
	Sphere->SetSphereRadius(30.f);

	// Sphereの位置を調整する
	Sphere->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f), false);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AVRDragon_ver2::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AVRDragon_ver2::OnSphereEndOverlap);

	// HMDの原点
	CameraRoot= CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(RootComponent);

	// Cameraを追加する
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(CameraRoot);

	// Input Mapping Context「IMC_VRDragon」を読み込む
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/VRTemplate/Input/IMC_VRDragon"));
	
	// Input Action「IA_DragonMove」を読み込む
	ControlMove = LoadObject<UInputAction>(nullptr, TEXT("/Game/VRTemplate/Input/Actions/Dragon/IA_DragonMove"));

	// Input Action「IA_DragonFire」を読み込む
	ControlFire = LoadObject<UInputAction>(nullptr, TEXT("/Game/VRTemplate/Input/Actions/Dragon/IA_DragonFire"));

	// Input Action「IA_DragonLook」を読み込む
	LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/VRTemplate/Input/Actions/Dragon/IA_DragonLook"));
	
	// Arrowの初期化
	{
		// Arrowを追加する
		Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
		Arrow->SetupAttachment(RootComponent);

		// Sphereの頭上に移動するようにLocationを設定する
		Arrow->SetRelativeLocation(FVector(400.0f, 0.0f, 130.0f));

		// Arrowを表示されるようにする
		Arrow->bHiddenInGame = false;
	}
}

// Called when the game starts or when spawned
void AVRDragon_ver2::BeginPlay()
{
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
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
void AVRDragon_ver2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FireChargeCnt < 0) {

		FireChargeCnt = 0;
	}

	if (FireChargeCnt > 0) {

		FireChargeCnt -= DeltaTime;
	}

	FString str = FString::SanitizeFloat(FireChargeCnt);

#if true
	{
		if (GEngine && GEngine->XRSystem.IsValid())
		{
			FQuat Orientation;
			FVector Position;
			if (GEngine->XRSystem->GetCurrentPose(IXRTrackingSystem::HMDDeviceId, Orientation, Position))
			{
				CameraRoot->SetRelativeLocationAndRotation(Position, Orientation);
			}
		}
	}
#endif
}

// Called to bind functionality to input
void AVRDragon_ver2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// ControlBallとIA_ControlのTriggeredをBindする
		EnhancedInputComponent->BindAction(ControlMove, ETriggerEvent::Triggered, this, &AVRDragon_ver2::ControlPlayer);

		// ControlBallとIA_ControlのTriggeredをBindする
		EnhancedInputComponent->BindAction(ControlFire, ETriggerEvent::Triggered, this, &AVRDragon_ver2::GoFire);

		// LookとIA_LookのTriggeredをBindする
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVRDragon_ver2::Look);
	}
}

// 接触処理

// コライダー同士が接触したときに呼び出される
void AVRDragon_ver2::OnSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult){


}

// コライダー同士が離れたときに呼び出される
void AVRDragon_ver2::OnSphereEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex) {

}

// 入力イベント

// プレイヤーのコントロール
void AVRDragon_ver2::ControlPlayer(const FInputActionValue& Value) {

	// inputのValueはVector2Dに変換できる
	const FVector2D V = Value.Get<FVector2D>();

	FVector PreLocation = GetActorLocation();
	FVector NewLocation = PreLocation + Arrow->GetComponentToWorld().TransformVectorNoScale(FVector(V.Y, V.X, 0.0f) * MoveSpeedPoint);

	SetActorLocation(NewLocation);

	
}

// 火球コントロール
void AVRDragon_ver2::GoFire(const FInputActionValue& Value) {

	if (const bool B = Value.Get<bool>()) {

		FireChargeCnt += GetWorld()->DeltaTimeSeconds * 2;

		if(FireChargeCnt >= 2.f)
		{
			FRotator look = GetControlRotation();
			look = Camera->GetComponentToWorld().GetRotation().Rotator();
			FVector pos = GetActorLocation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AFireBall_ver1* FireBall =
				GetWorld()->SpawnActor<AFireBall_ver1>(AFireBall_ver1::StaticClass(), pos, look); // スポーン処理 


			FireChargeCnt = 0;
		}
	}
}

// カメラコントロール
void AVRDragon_ver2::Look(const FInputActionValue& Value) {

	// inputのValueはVector2Dに変換できる
	FVector2D v = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(v.X);
		AddControllerPitchInput(v.Y);

		// Pawnが持っているControlの角度を取得する
		FRotator controlRotate = GetControlRotation();

		// カメラをまわす
		SetActorRotation(controlRotate);

		// 移動方向を指定する
		FRotator ArrowRotate = FRotator(0, controlRotate.Yaw, 0);
		Arrow->SetWorldRotation(ArrowRotate);
	}
}

// VRカメラ
bool AVRDragon_ver2::GetHMDPose(FVector& OutPosition, FRotator& OutRotation)
{
	if (GEngine && GEngine->XRSystem.IsValid())
    {
        IXRTrackingSystem* XRSystem = GEngine->XRSystem.Get();
        if (XRSystem->IsHeadTrackingAllowed())
        {
            // フレームに対するHMDの座標（ワールド空間かローカルかはシステムによる）
            FQuat OrientationQuat;
            FVector Position;

            // GetCurrentPose はコンポーネント（Head、LeftEye、RightEye）を指定して取得
            if (XRSystem->GetCurrentPose((int32)EXRTrackedDeviceType::HeadMountedDisplay, OrientationQuat, Position))
            {
                //OutPosition = Position;
                OutRotation = OrientationQuat.Rotator();
                return true;
            }
        }
    }
    return false;
}
