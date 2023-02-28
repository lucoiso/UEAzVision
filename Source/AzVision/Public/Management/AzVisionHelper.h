// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzVision

#pragma once

#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "AzVisionTypes.h"
#include "AzVisionHelper.generated.h"

/**
 * 
 */
UCLASS()
class AZVISION_API UAzVisionHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "AzVision")
	static FString SaveRenderTargetToImageFile(class UTextureRenderTarget2D* RenderTarget, const FString& OutputImagePath, const FString& OutputFileName);

	UFUNCTION(BlueprintPure, Category = "AzVision")
	static FAzVisionCustomVisionPrediction GetBetterPredictionValue(const TArray<FAzVisionCustomVisionPrediction>& Predictions);
};
