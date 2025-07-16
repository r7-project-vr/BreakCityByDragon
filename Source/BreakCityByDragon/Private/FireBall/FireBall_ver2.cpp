#include "FireBall/FireBall_ver2.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraComponent.h" // 新增：为UNiagaraComponent提供完整定义

// Sets default values
AFireBall_ver2::AFireBall_ver2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 火球模型组件
	FireBall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = FireBall;

	// 加载球体模型 (这部分保持不变，因为基础模型通常是固定的)
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));
	if (Mesh)
	{
		FireBall->SetStaticMesh(Mesh);
	}

	// 2. 碰撞组件
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName("BlockAll");
	SphereComponent->SetSphereRadius(30.0f);

	// 3. 动态红色材质 (这部分保持不变)
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

	// 4. 新增：创建拖尾组件 (但不设置具体资源)
	TrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailComponent"));
	TrailComponent->SetupAttachment(RootComponent);

	// 5. 绑定碰撞事件
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFireBall_ver2::OnHit);
}

// Called when the game starts or when spawned
void AFireBall_ver2::BeginPlay()
{
	Super::BeginPlay();

	// 在游戏开始时，应用在蓝图中设置的Niagara特效资源
	if (TrailEffectAsset && TrailComponent)
	{
		TrailComponent->SetAsset(TrailEffectAsset);
		TrailComponent->Activate(true); // 显式激活特效
	}
}

// Called every frame
void AFireBall_ver2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 移动逻辑保持不变
	const FVector NewLocation = GetActorLocation() + (GetActorRotation().Vector().GetSafeNormal() * 6000.f * DeltaTime);
	SetActorLocation(NewLocation);
}

void AFireBall_ver2::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// 防止火球与自身或场景触发器等非预期物体碰撞后立即销毁
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// 修改：添加命中后的逻辑
	// 1. 停止拖尾特效的粒子生成，让现有粒子自然消散
	if (TrailComponent)
	{
		TrailComponent->Deactivate();
	}

	// 2. 隐藏火球模型
	FireBall->SetVisibility(false);

	// 3. 关闭碰撞，防止在消失前再次触发
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 4. 停止移动 (可选，但推荐)
	PrimaryActorTick.bCanEverTick = false;

	// 5. 设置2秒后自动销毁Actor，给拖尾特效足够的时间播放完毕
	SetLifeSpan(2.0f);

	// TODO: 在这里添加对 OtherActor 造成伤害、播放爆炸声效等逻辑
}