// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Pin.h"


// Sets default values
APin::APin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pin"));
}

// Called when the game starts or when spawned
void APin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

