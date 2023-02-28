// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzVision

#include "Components/AzVisionSceneCapture2D.h"
#include "Management/AzVisionHelper.h"
#include "Management/AzVisionSettings.h"

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(AzVisionSceneCapture2D)
#endif

UAzVisionSceneCapture2D::UAzVisionSceneCapture2D(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCaptureEveryFrame = false;
	bCaptureOnMovement = false;
	PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	CaptureSource = ESceneCaptureSource::SCS_FinalToneCurveHDR;

	if (const UAzVisionSettings* Settings = UAzVisionSettings::Get())
	{
		TextureTarget = Settings->TextureRenderTarget.LoadSynchronous();
	}
}

FString UAzVisionSceneCapture2D::SaveRenderTargetToImageFile(const FString& OutputImagePath, const FString& OutputFileName)
{
	CaptureScene();
	return UAzVisionHelper::SaveRenderTargetToImageFile(TextureTarget, OutputImagePath, OutputFileName);
}
