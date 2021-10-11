// Copyright 2021 SharkoPuffs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "OnlineBlueprintFunctionsTypes.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "LogoutUserNumberCallbackProxy.generated.h"

class APlayerController;

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API ULogoutUserNumberCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// Called when Logged out successfully
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;

	// Called when Logout attempt fails
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

	// Attempt to Logout of the default Online Subsystem
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Identity")
	static ULogoutUserNumberCallbackProxy* LogoutUserNumber(UObject* WorldContextObject, class APlayerController* PlayerController, int32 PlayerNum);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	// Internal callback when logout attempt completes, calls OnSuccess or OnFailure
	void OnLogoutComplete(int32 LocalUserNum, bool bWasSuccessful);

	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// The delegate executed by the Online Subsystem
	FOnLogoutCompleteDelegate Delegate;

	// Handle to the registered delegate
	FDelegateHandle DelegateHandle;

	int32 PlayerNum;

	// The world context object in which this call is taking place
	UObject* WorldContextObject;
};
