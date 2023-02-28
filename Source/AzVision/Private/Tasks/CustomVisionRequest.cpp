// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzVision

#include "Tasks/CustomVisionRequest.h"
#include "Management/AzVisionSettings.h"
#include "LogAzVision.h"
#include <HttpModule.h>
#include <Interfaces/IHttpRequest.h>
#include <Interfaces/IHttpResponse.h>

#ifdef UE_INLINE_GENERATED_CPP_BY_NAME
#include UE_INLINE_GENERATED_CPP_BY_NAME(CustomVisionRequest)
#endif

UCustomVisionRequest* UCustomVisionRequest::SendCustomVisionRequestAsync(UObject* WorldContextObject, const FString& ImagePath, const EAzVisionCustomVisionMode Mode)
{
	UCustomVisionRequest* const Task = NewObject<UCustomVisionRequest>();
	Task->ImagePath = ImagePath;
	Task->Mode = Mode;
	Task->RegisterWithGameInstance(WorldContextObject);

	return Task;
}

void UCustomVisionRequest::Activate()
{
	Super::Activate();
	
	const UAzVisionSettings* const Settings = UAzVisionSettings::Get();

	if (!IsValid(Settings)								|| 
		Settings->CustomVisionPredictionKey.IsEmpty()	|| 
		Settings->CustomVisionSubscriptionKey.IsEmpty() ||
		Settings->CustomVisionEndpoint.IsEmpty()		||
		Settings->CustomVisionProjectID.IsEmpty()		||
		Settings->CustomVisionPublishedName.IsEmpty()	||
		Settings->CustomVisionApplication.IsEmpty())
	{
		UE_LOG(LogAzVision, Error, TEXT("%s (%d): Request not sent due to invalid params"), *FString(__func__), GetUniqueID());
		RequestNotSent.Broadcast();
		SetReadyToDestroy();
		return;
	}

	const FString Uri = MountURI();

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader("Prediction-key", Settings->CustomVisionPredictionKey);
	HttpRequest->SetHeader("Training-key", Settings->CustomVisionSubscriptionKey);
	HttpRequest->SetHeader("Content-Type", "application/octet-stream");
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Uri);
	HttpRequest->SetContentAsStreamedFile(ImagePath);

	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) -> void
	{
		FScopeLock Lock(&Mutex);

		if (IsValid(this))
		{
			ProcessResponse(Response->GetContentAsString(), bWasSuccessful);
		}
	});

	HttpRequest->ProcessRequest();
	RequestSent.Broadcast();
}

void UCustomVisionRequest::ProcessResponse(const FString& Content, const bool bWasSuccessful)
{
	FScopeLock Lock(&Mutex);

	if (!bWasSuccessful)
	{
		UE_LOG(LogAzVision, Error, TEXT("%s (%d): Request failed"), *FString(__func__), GetUniqueID());
		return;
	}

	UE_LOG(LogAzVision, Display, TEXT("%s (%d): Response Received"), *FString(__func__), GetUniqueID());
	UE_LOG(LogAzVision_Internal, Display, TEXT("%s (%d): Response: %s"), *FString(__func__), GetUniqueID(), *Content);

	ResponseReceived.Broadcast(GetDesserializedResponseString(Content));
	SetReadyToDestroy();
}

FAzVisionCustomVisionResponse UCustomVisionRequest::GetDesserializedResponseString(const FString& Content) const
{
	FAzVisionCustomVisionResponse Output;

	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
	TSharedPtr<FJsonObject> JsonResponse = MakeShareable(new FJsonObject);
	FJsonSerializer::Deserialize(Reader, JsonResponse);

	if (JsonResponse.IsValid())
	{
		if (const TSharedPtr<FJsonObject>* Error; JsonResponse->TryGetObjectField("error", Error))
		{
			return Output;
		}

		if (FString Code; JsonResponse->TryGetStringField("code", Code))
		{
			return Output;
		}

		Output.ID = JsonResponse->GetStringField("id");
		Output.Project = JsonResponse->GetStringField("project");
		Output.Iteration = JsonResponse->GetStringField("iteration");
		Output.Created = JsonResponse->GetStringField("created");

		const TArray<TSharedPtr<FJsonValue>> Predictions = JsonResponse->GetArrayField("predictions");
		for (const TSharedPtr<FJsonValue>& Prediction : Predictions)
		{
			FAzVisionCustomVisionPrediction PredictionOutput;
			PredictionOutput.TagID = Prediction->AsObject()->GetStringField("tagId");
			PredictionOutput.TagName = Prediction->AsObject()->GetStringField("tagName");
			PredictionOutput.Probability = Prediction->AsObject()->GetNumberField("probability");

			if (const TSharedPtr<FJsonObject>* BoundingBox; Prediction->AsObject()->TryGetObjectField("boundingBox", BoundingBox))
			{
				FAzVisionCustomVisionBoundingBox BoundingBoxOutput;
				BoundingBoxOutput.Left = (*BoundingBox)->GetNumberField("left");
				BoundingBoxOutput.Top = (*BoundingBox)->GetNumberField("top");
				BoundingBoxOutput.Width = (*BoundingBox)->GetNumberField("width");
				BoundingBoxOutput.Height = (*BoundingBox)->GetNumberField("height");

				PredictionOutput.BoundingBox = BoundingBoxOutput;
			}

			Output.Predictions.Add(PredictionOutput);
		}
	}

    return Output;
}

FString UCustomVisionRequest::MountURI() const
{	
	const UAzVisionSettings* const Settings = UAzVisionSettings::Get();
	if (!IsValid(Settings))
	{
		UE_LOG(LogAzVision, Error, TEXT("%s (%d): Failed to mount request URI due to invalid settings."), *FString(__func__), GetUniqueID());
		return FString();
	}

	FString ModeStr;
	switch (Mode)
	{
		case EAzVisionCustomVisionMode::Classify:
			ModeStr = "classify";
			break;

		case EAzVisionCustomVisionMode::Detect:
			ModeStr = "detect";
			break;

		default: 
			return FString(); // Invalid
	}

	const FStringFormatOrderedArguments Arguments {
		Settings->CustomVisionEndpoint,
		Settings->CustomVisionProjectID,
		ModeStr,
		Settings->CustomVisionPublishedName,
		Settings->CustomVisionApplication
	};

	return FString::Format(TEXT("{0}/customvision/v3.1/Prediction/{1}/{2}/iterations/{3}/image?application={4}"), Arguments);
}
