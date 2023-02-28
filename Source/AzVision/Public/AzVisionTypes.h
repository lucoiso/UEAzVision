// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzVision

#pragma once

#include <CoreMinimal.h>
#include "AzVisionTypes.generated.h"

/**
 *
 */

USTRUCT(BlueprintType, Category = "AzVision | Custom Vision", Meta = (DisplayName = "Custom Vision Bounding Box"))
struct FAzVisionCustomVisionBoundingBox
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	float Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	float Top;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	float Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	float Height;
};

USTRUCT(BlueprintType, Category = "AzVision | Custom Vision", Meta = (DisplayName = "Custom Vision Prediction"))
struct FAzVisionCustomVisionPrediction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision", Meta = (DisplayName = "Tag ID"))
	FString TagID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	FString TagName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	float Probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	FAzVisionCustomVisionBoundingBox BoundingBox;
};

USTRUCT(BlueprintType, Category = "AzVision | Custom Vision", Meta = (DisplayName = "Custom Vision Response"))
struct FAzVisionCustomVisionResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision", Meta = (DisplayName = "ID"))
	FString ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	FString Project;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	FString Iteration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	FString Created;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzVision | Custom Vision")
	TArray<FAzVisionCustomVisionPrediction> Predictions;
};

UENUM(BlueprintType, Category = "AzVision | Custom Vision")
enum class EAzVisionCustomVisionMode : uint8
{
	Classify,
	Detect
};