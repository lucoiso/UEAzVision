// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzVision

#include "Management/AzVisionHelper.h"
#include "Management/AzVisionSettings.h"
#include "LogAzVision.h"
#include <Engine/TextureRenderTarget2D.h>
#include <ImageWriteBlueprintLibrary.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(AzVisionHelper)
#endif

FString UAzVisionHelper::SaveRenderTargetToImageFile(UTextureRenderTarget2D* RenderTarget, const FString& OutputImagePath, const FString& OutputFileName)
{
	const UAzVisionSettings* const Settings = UAzVisionSettings::Get();
	if (!IsValid(RenderTarget) || !IsValid(Settings) || OutputImagePath.IsEmpty() || OutputFileName.IsEmpty())
	{
		return FString();
	}

	FString OutputFileExtension;

	switch (Settings->ImageFormat)
	{
		case EDesiredImageFormat::BMP:
			OutputFileExtension = ".bmp";
			break;

		case EDesiredImageFormat::EXR:
			OutputFileExtension = ".exr";
			break;

		case EDesiredImageFormat::JPG:
			OutputFileExtension = ".jpg";
			break;

		case EDesiredImageFormat::PNG:
			OutputFileExtension = ".png";
			break;

		default: 
			return FString(); // Invalid
	}

	FString QualifiedPath = FPaths::Combine(OutputImagePath, OutputFileName + OutputFileExtension);
	FPaths::NormalizeFilename(QualifiedPath);

	FImageWriteOptions IW_Options;
	IW_Options.bAsync = true;
	IW_Options.bOverwriteFile = true;
	IW_Options.CompressionQuality = Settings->CompressionQuality;
	IW_Options.Format = Settings->ImageFormat;

	TFunction<void(bool)> CompletionResult = [QualifiedPath, FuncName = __func__](const bool Result) -> bool
	{
		UE_LOG(LogAzVision, Warning, TEXT("%s - Result: %u; Path: %s"), *FString(FuncName), Result, *QualifiedPath);
		return Result;
	};

	IW_Options.NativeOnComplete = CompletionResult;
	UImageWriteBlueprintLibrary::ExportToDisk(RenderTarget, QualifiedPath, IW_Options);

	return QualifiedPath;
}

FAzVisionCustomVisionPrediction UAzVisionHelper::GetBetterPredictionValue(const TArray<FAzVisionCustomVisionPrediction>& Predictions)
{
	if (Predictions.IsEmpty())
	{
		return FAzVisionCustomVisionPrediction();
	}

	FAzVisionCustomVisionPrediction BetterPrediction = Predictions[0];

	for (const FAzVisionCustomVisionPrediction& Prediction : Predictions)
	{
		if (Prediction.Probability > BetterPrediction.Probability)
		{
			BetterPrediction = Prediction;
		}
	}

	return BetterPrediction;
}
