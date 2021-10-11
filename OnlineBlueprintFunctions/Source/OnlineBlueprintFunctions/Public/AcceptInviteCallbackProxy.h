// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "LoginCallbackProxy.h"
#include "GetFriendsListCallbackProxy.h"
#include "AcceptInviteCallbackProxy.generated.h"

class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlueprintOnAcceptInviteComplete, const FString&, ErrorStr);

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API UAcceptInviteCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FBlueprintOnAcceptInviteComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FBlueprintOnAcceptInviteComplete OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Friends")
	static UAcceptInviteCallbackProxy* AcceptInvite(UObject* WorldContextObject, class APlayerController* PlayerController, int32 LocalUserNum, FBlueprintOnlineFriend Friend, EBlueprintFriendsListType ListName);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	void OnAcceptInviteComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& FriendListName, const FString& ErrorStr);

	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	int32 LocalUserNumber;
	FBlueprintOnlineFriend Friend;
	EBlueprintFriendsListType ListName;

	UObject* WorldContextObject;
};
