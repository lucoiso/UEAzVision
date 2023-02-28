// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzVision

#include "Management/AzVisionSettings.h"
#include "LogAzVision.h"

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(AzVisionSettings)
#endif

UAzVisionSettings::UAzVisionSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), ImageFormat(EDesiredImageFormat::PNG), CompressionQuality(0)
{
	CategoryName = TEXT("Plugins");

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderObj(TEXT("/Script/Engine.TextureRenderTarget2D'/AzVision/RTX_Main_VisionTarget.RTX_Main_VisionTarget'"));
	if (RenderObj.Succeeded())
	{
		TextureRenderTarget = RenderObj.Object;
	}
}

const UAzVisionSettings* UAzVisionSettings::Get()
{
	static const UAzVisionSettings* const Instance = GetDefault<UAzVisionSettings>();
	return Instance;
}

#if WITH_EDITOR
void UAzVisionSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAzVisionSettings, bEnableInternalLogs))
	{
		ToggleInternalLogs();
	}
}
#endif

void UAzVisionSettings::PostInitProperties()
{
	Super::PostInitProperties();
	ToggleInternalLogs();
}

void UAzVisionSettings::ToggleInternalLogs()
{
#if !UE_BUILD_SHIPPING
	LogAzVision_Internal.SetVerbosity(bEnableInternalLogs ? ELogVerbosity::Display : ELogVerbosity::NoLogging);
#endif
}
