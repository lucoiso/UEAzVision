// Author: Lucas Vilas-Boas
// Year: 2023
// Repo: https://github.com/lucoiso/UEAzVision

#pragma once

#include <CoreMinimal.h>
#include <Kismet/BlueprintAsyncActionBase.h>
#include "AzVisionTypes.h"
#include "CustomVisionRequest.generated.h"

/**
 *
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAzVisionCustomVisionGenericDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAzVisionCustomVisionResponseDelegate, const FAzVisionCustomVisionResponse&, Result);

/**
 * 
 */
UCLASS(NotPlaceable, Category = "AzVision | Custom Vision")
class AZVISION_API UCustomVisionRequest : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "AzSpeech")
	FAzVisionCustomVisionResponseDelegate ResponseReceived;

	UPROPERTY(BlueprintAssignable, Category = "AzSpeech")
	FAzVisionCustomVisionGenericDelegate RequestSent;

	UPROPERTY(BlueprintAssignable, Category = "AzSpeech")
	FAzVisionCustomVisionGenericDelegate RequestNotSent;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Send Custom Vision Request Async"))
	static UCustomVisionRequest* SendCustomVisionRequestAsync(UObject* WorldContextObject, const FString& ImagePath, const EAzVisionCustomVisionMode Mode = EAzVisionCustomVisionMode::Classify);

	virtual void Activate() override;

protected:
	mutable FCriticalSection Mutex;

	FString ImagePath;
	EAzVisionCustomVisionMode Mode;

	virtual void ProcessResponse(const FString& Content, const bool bWasSuccessful);
	FAzVisionCustomVisionResponse GetDesserializedResponseString(const FString& Content) const;

	FString MountURI() const;
};
