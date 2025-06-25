#include "FireBall/FireBall_ver2.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AFireBall_ver2::AFireBall_ver2()
{
	PrimaryActorTick.bCanEverTick = true;

	// 火球模型组件
	FireBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = FireBall;

	// 加载球体模型
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));
	if (Mesh)
	{
		FireBall->SetStaticMesh(Mesh);
	}

	// 创建碰撞组件
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("BlockAll");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(30.0f); // 稍大一点

	// 创建红色动态材质
	UMaterial* BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (BaseMaterial)
	{
		UMaterialInstanceDynamic* RedMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		if (RedMaterial)
		{
			RedMaterial->SetVectorParameterValue("Color", FLinearColor::Red);
			FireBall->SetMaterial(0, RedMaterial);
		}
	}

	// 绑定碰撞事件
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFireBall_ver2::OnHit);
}

// Called when the game starts or when spawned
void AFireBall_ver2::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFireBall_ver2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 稍微慢一点
	FVector NewLocation = GetActorLocation() + (GetActorRotation().Vector().GetSafeNormal() * 6000.f * DeltaTime);
	SetActorLocation(NewLocation);
}

void AFireBall_ver2::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// 暂时空实现，可添加爆炸、销毁等逻辑
}
