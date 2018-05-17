// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pin.h"
#include "PinGenerator.generated.h"

UCLASS()
class IRISTEST_API APinGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APinGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TSubclassOf<class APin> ToSpawn;
	
	UTexture2D * texture;

	TArray<float> colorValues;

	UFUNCTION()
	void Spawn(int width, int height);

	UFUNCTION()
	void CreateTextureValues(FString path);

	//UPROPERTY(EditAnywhere)
	float offsetX = 100.0f;
	//UPROPERTY(EditAnywhere)
	float offsetY = 100.0f;

};
