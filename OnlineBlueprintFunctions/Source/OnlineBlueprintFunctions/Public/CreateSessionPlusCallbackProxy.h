// Copyright 2021 SharkoPuffs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineBlueprintFunctionsTypes.h"
#include "GameFramework/OnlineReplStructs.h"
#include "CreateSessionPlusCallbackProxy.generated.h"

class APlayerController;

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API UCreateSessionPlusCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Session")
	static UCreateSessionPlusCallbackProxy* CreateSessionPlus(UObject* WorldContextObject, class APlayerController* PlayerController, ESessionNamesType SessionType, FBlueprintSessionSettings Settings);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	FOnCreateSessionCompleteDelegate Delegate;

	FDelegateHandle DelegateHandle;

	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;
	ESessionNamesType SessionType;
	FBlueprintSessionSettings Settings;
	UObject* WorldContextObject;
};
