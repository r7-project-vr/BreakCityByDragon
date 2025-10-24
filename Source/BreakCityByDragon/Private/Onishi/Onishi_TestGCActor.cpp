// Fill out your copyright notice in the Description page of Project Settings.

#include "Onishi/Onishi_TestGCActor.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "UObject/ConstructorHelpers.h"

AOnishi_TestGCActor::AOnishi_TestGCActor() {


}

void AOnishi_TestGCActor::BeginPlay() {

    UWorld* World = GetWorld();
    if (!World) return;

    // GCアセットをC++で取得（手動バインド）
    UGeometryCollection* GCAsset = LoadObject<UGeometryCollection>(
        nullptr,
        TEXT("/Game/Onishi/test/middletest_1.middletest_1")
    );

    if (!GCAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("GCアセットの読み込みに失敗しました"));
        return;
    }

    // アクターをスポーン
    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = FName("MyDestructibleGCActor");

    AGeometryCollectionActor* GCActor = World->SpawnActor<AGeometryCollectionActor>(
        AGeometryCollectionActor::StaticClass(),
        FVector(0, 0, 300), // 任意の位置
        FRotator::ZeroRotator,
        SpawnParams
    );

    if (GCActor && GCActor->GetGeometryCollectionComponent())
    {
        UGeometryCollectionComponent* GCComp = GCActor->GetGeometryCollectionComponent();

        GCComp->SetRestCollection(GCAsset);                     // ← アセットをバインド
        GCComp->SetSimulatePhysics(true);                      // 物理ON
        GCComp->SetNotifyRigidBodyCollision(true);             // 衝突通知ON
        GCComp->SetEnableGravity(false);
        GCComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        GCComp->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

        UE_LOG(LogTemp, Log, TEXT("ジオメトリコレクションアクターをスポーンしました"));
    }

}