// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzVision

#pragma once

#include <CoreMinimal.h>
#include <Components/SceneCaptureComponent2D.h>
#include "AzVisionSceneCapture2D.generated.h"

/**
 * 
 */
UCLASS()
class AZVISION_API UAzVisionSceneCapture2D : public USceneCaptureComponent2D
{
	GENERATED_BODY()
	
public:
	explicit UAzVisionSceneCapture2D(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "AzVision")
	FString SaveRenderTargetToImageFile(const FString& OutputImagePath, const FString& OutputFileName);
};
