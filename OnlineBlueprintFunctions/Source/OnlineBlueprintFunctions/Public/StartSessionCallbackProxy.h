// Copyright 2021 SharkoPuffs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "OnlineBlueprintFunctionsTypes.h"
#include "StartSessionCallbackProxy.generated.h"

class APlayerController;

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API UStartSessionCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Session")
	static UStartSessionCallbackProxy* StartSession(UObject* WorldContextObject, class APlayerController* PlayerController, ESessionNamesType SessionType);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:

	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

	FOnStartSessionCompleteDelegate Delegate;

	FDelegateHandle DelegateHandle;

	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	ESessionNamesType SessionType;

	UObject* WorldContextObject;
};
