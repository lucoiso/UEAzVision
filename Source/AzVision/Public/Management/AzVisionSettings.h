// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzVision

#pragma once

#include <CoreMinimal.h>
#include <Engine/DeveloperSettings.h>
#include <ImageWriteTypes.h>
#include "AzVisionSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Plugins, DefaultConfig, meta = (DisplayName = "AzVision"))
class AZVISION_API UAzVisionSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	explicit UAzVisionSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	static const UAzVisionSettings* Get();
	
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Custom Vision", Meta = (DisplayName = "Application"))
	FString CustomVisionApplication;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Custom Vision", Meta = (DisplayName = "Subscription Key"))
	FString CustomVisionSubscriptionKey;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Custom Vision", Meta = (DisplayName = "Project ID"))
	FString CustomVisionProjectID;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Custom Vision", Meta = (DisplayName = "Published Name"))
	FString CustomVisionPublishedName;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Custom Vision", Meta = (DisplayName = "Endpoint"))
	FString CustomVisionEndpoint;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Custom Vision", Meta = (DisplayName = "Prediction Key"))
	FString CustomVisionPredictionKey;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Rendering", Meta = (DisplayName = "Texture Render Target"))
	TSoftObjectPtr<class UTextureRenderTarget2D> TextureRenderTarget;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Rendering", Meta = (DisplayName = "Image Format"))
	EDesiredImageFormat ImageFormat;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Rendering", Meta = (DisplayName = "Compression Quality"))
	int32 CompressionQuality;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "Logging", Meta = (DisplayName = "Enable Internal Logs"))
	bool bEnableInternalLogs;

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void PostInitProperties() override;

private:
	void ToggleInternalLogs();
};
