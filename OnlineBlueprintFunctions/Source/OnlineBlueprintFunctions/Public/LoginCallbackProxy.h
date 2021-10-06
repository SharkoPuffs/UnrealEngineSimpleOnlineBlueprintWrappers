#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "LoginCallbackProxy.generated.h"

class APlayerController;

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API ULoginCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// Called when Logged in successfully
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;

	// Called when Login attempt fails
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

	// Attempt to Login to the default Online Subsystem
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Identity")
	static ULoginCallbackProxy* Login(UObject* WorldContextObject, class APlayerController* PlayerController, FString UserName, FString Password, FString Type, int32 PlayerNum);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	// Internal callback when login attempt completes, calls
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrString);

	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// The delegate executed by the Online Subsystem
	FOnLoginCompleteDelegate LoginCompleteDelegate;

	// Handle to the registered delegate
	FDelegateHandle LoginCompleteDelegateHandle;

	// Login details
	FString UserName;
	FString Password;
	FString Type;
	int32 PlayerNum;

	// The world context object in which this call is taking place
	UObject* WorldContextObject;
};