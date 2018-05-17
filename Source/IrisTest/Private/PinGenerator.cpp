// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/PinGenerator.h"
#include "Engine.h"

// Sets default values
APinGenerator::APinGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<APin> BPClass(TEXT("/Game/PinObject"));
	if (BPClass.Succeeded())
	{
		ToSpawn = BPClass.Class;
	}
}
// Called when the game starts or when spawned
void APinGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APinGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APinGenerator::Spawn(int width, int height)
{

	uint32 colorCounter = 0;

	if (ToSpawn)
	{
		UWorld * world = GetWorld();

		if (world)
		{
			FActorSpawnParameters spawnParameters;
			spawnParameters.Owner = this;

			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					FRotator rotator = FRotator::ZeroRotator;
					FVector spawnLocation = GetActorLocation() + FVector((i * offsetX), (j * offsetY), 0.0f);
					APin * newPin = world->SpawnActor<APin>(ToSpawn, spawnLocation, rotator, spawnParameters);
					FString colorValuesCount = FString::FromInt(colorValues.Num());
					UE_LOG(LogTemp, Warning, TEXT("%s"),*colorValuesCount);
					newPin->GetActorTransform().SetScale3D(FVector(1.0f, 1.0f, colorValues[colorCounter]));
					colorCounter++;
				}
			}

		}

	}
}

void APinGenerator::CreateTextureValues(FString path)
{
	FString contentPath = "/Game/Images/";
	contentPath.Append(*path);

	texture = LoadObject<UTexture2D>( nullptr, *(contentPath));

	if (texture)
	{
		FTexture2DMipMap* MyMipMap = &texture->PlatformData->Mips[0];
		FByteBulkData* RawImageData = &MyMipMap->BulkData;

		FColor* FormatedImageData = static_cast<FColor*>(RawImageData->Lock(LOCK_READ_ONLY));

		uint32 TextureWidth = MyMipMap->SizeX, TextureHeight = MyMipMap->SizeY;
		TArray<FColor> PixelColors;
		uint32 pixelCounter = 0;

		PixelColors.SetNum(TextureWidth * TextureHeight, true);
		colorValues.SetNum(TextureWidth * TextureHeight,true);

		for (uint32 i = 0; i < TextureWidth; i++)
		{
			for (uint32 j = 0; j < TextureHeight; j++)
			{
				PixelColors[pixelCounter] = FormatedImageData[j * TextureWidth + i];
				colorValues[pixelCounter] = (PixelColors[pixelCounter].R + PixelColors[pixelCounter].G + PixelColors[pixelCounter].B);
				pixelCounter++;
			}
		}

		RawImageData->Unlock();
	}
	
}

