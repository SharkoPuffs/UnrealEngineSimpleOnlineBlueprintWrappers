// Copyright 2021 SharkoPuffs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "OnlineBlueprintFunctionsTypes.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "DestroyNamedSessionCallbackProxy.generated.h"

class APlayerController;

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API UDestroyNamedSessionCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Session")
	static UDestroyNamedSessionCallbackProxy* DestroyNamedSession(UObject* WorldContextObject, class APlayerController* PlayerController, ESessionNamesType SessionType);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FOnDestroySessionCompleteDelegate Delegate;

	FDelegateHandle DelegateHandle;

	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	ESessionNamesType SessionType;
	UObject* WorldContextObject;
};
