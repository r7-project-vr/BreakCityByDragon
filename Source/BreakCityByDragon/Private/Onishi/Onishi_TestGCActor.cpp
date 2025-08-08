// Fill out your copyright notice in the Description page of Project Settings.


#include "Onishi/Onishi_TestGCActor.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "UObject/ConstructorHelpers.h"

AOnishi_TestGCActor::AOnishi_TestGCActor() {

    //GeometryCollection‚ÌƒoƒCƒ“ƒh
    static ConstructorHelpers::FObjectFinder<UGeometryCollection> GeoCollectionAsset(
        TEXT("/Game/Onishi/test/middletest_1.middletest_1")
    );

    //GCComponent=
}
