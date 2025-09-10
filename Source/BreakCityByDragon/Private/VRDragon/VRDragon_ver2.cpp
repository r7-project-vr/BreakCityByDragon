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
#include "FireBall/FireBall_ver2.h"
// VR
#include "Engine/Engine.h"
#include "IXRTrackingSystem.h"
#include "HeadMountedDisplay.h"

// Sets default values
AVRDragon_ver2::AVRDragon_ver2() :
	FireChargeCnt(0),
	CanFire(false),
	preTailVec(0, 0, 0),
	newTailVec(0, 0, 0)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root;

	// StaticMeshComponentを追加し、RootComponentに設定する
	Player = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Player->SetupAttachment(RootComponent);

	// Sphere
	{
		// SphereComponentを追加し
		Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		Sphere->SetupAttachment(RootComponent);

		// Sphereのサイズを設定する
		Sphere->SetSphereRadius(30.f);

		// Sphereの位置を調整する
		Sphere->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f), false);

		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AVRDragon_ver2::OnSphereBeginOverlap);
		Sphere->OnComponentEndOverlap.AddDynamic(this, &AVRDragon_ver2::OnSphereEndOverlap);
	}

	// Box_body
	{
		// メッシュの生成
		UStaticMesh* Box = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
		Body->SetWorldScale3D(FVector(0.7f, 0.6f, 1.0f));
		Body->SetupAttachment(RootComponent);
		//Body->SetStaticMesh(Box);

		// Material
		UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
		//Body->SetMaterial(0, Material);

		// 接触判定
		Body_Base = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
		Body_Base->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f), false);
		Body_Base->SetupAttachment(Body);
		Body_Base->SetWorldScale3D(FVector(0.7f, 0.6f, 1.0f));
		Body_Base->OnComponentBeginOverlap.AddDynamic(this, &AVRDragon_ver2::OnSphereBeginOverlap);
		Body_Base->OnComponentEndOverlap.AddDynamic(this, &AVRDragon_ver2::OnSphereEndOverlap);
	}

	// Camera
	{
		// HMDの原点
		CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
		CameraRoot->SetupAttachment(RootComponent);

		// Cameraを追加する
		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
		Camera->SetupAttachment(CameraRoot);
	}

	// 脚
	{
		/*UStaticMesh* Box = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));

		LFootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LFootMeshComponent"));
		LFootMesh->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
		LFootMesh->SetupAttachment(RootComponent);
		LFootMesh->SetRelativeLocation(FVector(50.0f, 20.0f, -100.0f));
		LFootMesh->SetStaticMesh(Box);*/
	}

	// VRコントローラ
	{
		UStaticMesh* _Sphere = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));

		// 左手
		LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
		LeftMotionController->SetupAttachment(RootComponent);
		LeftMotionController->SetTrackingSource(EControllerHand::Left);
		LMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftMesh"));
		LMesh->SetupAttachment(LeftMotionController);
		LMesh->SetStaticMesh(_Sphere);
		LMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));

		// 右手
		RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
		RightMotionController->SetupAttachment(RootComponent);
		RightMotionController->SetTrackingSource(EControllerHand::Right);
		RMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightMesh"));
		RMesh->SetupAttachment(RightMotionController);
		RMesh->SetStaticMesh(_Sphere);
		RMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	}

	// エンハンス何とか
	{
		// Input Mapping Context「IMC_VRDragon」を読み込む
		DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/VRTemplate/Input/IMC_VRDragon"));

		// Input Action「IA_DragonMove」を読み込む
		ControlMove = LoadObject<UInputAction>(nullptr, TEXT("/Game/VRTemplate/Input/Actions/Dragon/IA_DragonMove"));

		// Input Action「IA_DragonFire」を読み込む
		ControlFire = LoadObject<UInputAction>(nullptr, TEXT("/Game/VRTemplate/Input/Actions/Dragon/IA_DragonFire"));

		// Input Action「IA_DragonLook」を読み込む
		LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/VRTemplate/Input/Actions/Dragon/IA_DragonLook"));
	}

	// Arrowの初期化
	{
		// Arrowを追加する
		Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
		Arrow->SetupAttachment(CameraRoot);

		// Sphereの頭上に移動するようにLocationを設定する
		Arrow->SetRelativeLocation(FVector(400.0f, 0.0f, 130.0f));

		// Arrowを表示されるようにする
		Arrow->bHiddenInGame = false;
	}

	// FireBall
	{
		static ConstructorHelpers::FClassFinder<AActor> BPClass(TEXT("/Game/Level/Han/BP_MyFireBall_ver2"));
		if (BPClass.Succeeded())
		{
			BlueprintFireBall = BPClass.Class;
		}
	}

	// UI
	{
		static ConstructorHelpers::FClassFinder<UDamageText> WidgetClass(TEXT("/Game/WBP_TEstDamageTExt"));
		if (WidgetClass.Succeeded())
		{
			DamageTextClass = WidgetClass.Class;
		}
	}
}

// Called when the game starts or when spawned
void AVRDragon_ver2::BeginPlay()
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

	// ASerialReceiverActorの生成
	{
		FRotator r = FRotator::ZeroRotator;
		FVector v = FVector::ZeroVector;
		ASerialReceiverActor = GetWorld()->SpawnActor<AASerialReceiverActor>(AASerialReceiverActor::StaticClass(), v, r);
	}

	// 尻尾の生成
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator look = GetControlRotation();
		look = Camera->GetComponentToWorld().GetRotation().Rotator() + FRotator(0, -90.f, 0);
		FVector pos = GetActorLocation() + FVector(-20.f, 0, 0);

		tails = GetWorld()->SpawnActor<ATailActor_ver2>(ATailActor_ver2::StaticClass(), pos, look, SpawnParams);
		tails->GetRootComponent()->SetupAttachment(RootComponent);
		tails->SerialReceiver = ASerialReceiverActor;
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

	// VRのカメラ
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

	// 尻尾に応じて動かす
	MovePlayer();
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
	const FHitResult& SweepResult) {


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

	//newTailVec = FVector(V.X, V.Y, 0);
}

// 火球コントロール
void AVRDragon_ver2::GoFire(const FInputActionValue& Value) {

	if (const bool B = Value.Get<bool>()) {

		FireChargeCnt += GetWorld()->DeltaTimeSeconds * 2;

		if (DamageTextClass)
		{
			UDamageText* DamageText = CreateWidget<UDamageText>(GetWorld(), DamageTextClass);
			if (DamageText)
			{
				DamageText->AddToViewport();
				DamageText->SetDesiredSizeInViewport(FVector2D(200.f, 100.f));
			}
		}


		if (FireChargeCnt >= 2.f)
		{
			FRotator look = GetControlRotation();
			look = Camera->GetComponentToWorld().GetRotation().Rotator();
			FVector pos = GetActorLocation() + GetActorForwardVector() * 160;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			GetWorld()->SpawnActor<AActor>(BlueprintFireBall, pos, look); // スポーン処理 

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

void AVRDragon_ver2::CheckVec(FVector& v_) {

	// マップの処理
	if (v_.X > 6000.f)		{ v_.X = 6000.f; }
	if (v_.X < -18000.f)	{ v_.X = -18000.f;}
	if (v_.Y > 14700.f)		{ v_.Y = 14700.f; }
	if (v_.Y < -14700.f)	{ v_.Y = -14700.f; }

	// ボスの処理
	if (v_.X > 4050.f) { v_.X = 4050.f; }
	if (v_.X < -350.f) { v_.X = -350.f; }
	if (v_.Y > 3300.f) { v_.Y = 3300.f; }
	if (v_.Y < -3100.f) { v_.Y = -3100.f; }
}

void AVRDragon_ver2::MovePlayer() {

	// 尻尾に合わせて動かす
	{
		newTailVec =
		{
			ASerialReceiverActor->GetRotation(3).Roll,
			ASerialReceiverActor->GetRotation(3).Pitch,
			ASerialReceiverActor->GetRotation(3).Yaw
		};

		FString s = newTailVec.ToString();
		UE_LOG(LogTemp, Log, TEXT("newTailVec : %s"),*s)
		FVector p = newTailVec - preTailVec;

		float pow[3] = {

			p.X,
			p.Y,
			p.Z
		};

		float addpow = 0;

		for (int n = 0; n < 3; n++) {

			if (pow[n] < 0) pow[n] *= -1;

			pow[n] /= 360.f;

			addpow += pow[n];
		}

		if (addpow < 0.0003f) { addpow = 0; }

		FVector PreLocation = GetActorLocation();
		FVector Forward = {
			FVector::ForwardVector.X,
			FVector::ForwardVector.Y,
			0
		};
		FVector NewLocation = PreLocation + Arrow->GetComponentToWorld().TransformVectorNoScale(Forward * MoveSpeedPoint * addpow);

		CheckVec(NewLocation);
		SetActorLocation(NewLocation);

		preTailVec = newTailVec;
	}
}

// しっぽの付け根を返す
FVector AVRDragon_ver2::GetTargetActorLocation() {

	return GetActorLocation();
}

// しっぽの角度を返す
FRotator AVRDragon_ver2::GetTargetActorRotation() {

	return GetActorRotation();
}

// アクターの前方方向のベクトルを返す
FVector AVRDragon_ver2::GetTargetActorForwardVector() {

	return GetActorForwardVector();
}