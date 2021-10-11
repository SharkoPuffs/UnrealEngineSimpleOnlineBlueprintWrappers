// Copyright 2021 SharkoPuffs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineBlueprintFunctionsTypes.h"
#include "FindSessionsCallbackProxy.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "JoinSessionPlusCallbackProxy.generated.h"

class APlayerController;

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API UJoinSessionPlusCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// Called when there is a successful join
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;

	// Called when there is an unsuccessful join
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Session")
	static UJoinSessionPlusCallbackProxy* JoinSessionPlus(UObject* WorldContextObject, class APlayerController* PlayerController, ESessionNamesType SessionType, const FBlueprintSessionResult& SearchResult);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	// Internal callback when the join completes, calls out to the public success/failure callbacks
	void OnCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// The search result we are sttempting to join
	FOnlineSessionSearchResult OnlineSearchResult;

	// The delegate executed by the online subsystem
	FOnJoinSessionCompleteDelegate Delegate;

	// Handle to the registered FOnJoinSessionComplete delegate
	FDelegateHandle DelegateHandle;

	// The world context object in which this call is taking place
	UObject* WorldContextObject;

	ESessionNamesType SessionType;
};
