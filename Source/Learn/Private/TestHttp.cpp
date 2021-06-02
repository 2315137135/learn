// Fill out your copyright notice in the Description page of Project Settings.


#include "TestHttp.h"

// Sets default values
ATestHttp::ATestHttp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestHttp::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATestHttp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

