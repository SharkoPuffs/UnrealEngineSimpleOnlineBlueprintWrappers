// Copyright 2021 SharkoPuffs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineBlueprintFunctionsTypes.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "FindSessionsCallbackProxy.h"
#include "FindSessionsPlusCallbackProxy.generated.h"

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API UFindSessionsPlusCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()
	
	// Called when there is a successful query
	UPROPERTY(BlueprintAssignable)
	FBlueprintFindSessionsResultDelegate OnSuccess;

	// Called when there is an unsuccessful query
	UPROPERTY(BlueprintAssignable)
	FBlueprintFindSessionsResultDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Session")
	static UFindSessionsPlusCallbackProxy* FindSessionsPlus(UObject* WorldContextObject, class APlayerController* PlayerController, int32 MaxResults, bool bUseLAN, FBlueprintSearchSettings SearchSettings);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	// Internal callback when the session search completes, calls out to the public success/failure callbacks
	void OnFindSessionsComplete(bool bWasSuccessful);

private:
	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// The delegate executed by the online subsystem
	FOnFindSessionsCompleteDelegate Delegate;

	// Handle to the registered OnFindSessionsComplete delegate
	FDelegateHandle DelegateHandle;

	// Object to track search results
	TSharedPtr<FOnlineSessionSearch> SearchObject;

	// Whether or not to search LAN
	bool bUseLAN;

	// Maximum number of results to return
	int MaxResults;

	// The world context object in which this call is taking place
	UObject* WorldContextObject;

	FBlueprintSearchSettings SearchSettings;
};
