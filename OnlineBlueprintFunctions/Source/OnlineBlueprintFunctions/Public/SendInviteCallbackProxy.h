// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "GetFriendsListCallbackProxy.h"
#include "SendInviteCallbackProxy.generated.h"

class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlueprintOnSendInviteCompleteDelegate, const FString&, ErrorStr);

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API USendInviteCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

	// Called when invite sent successfully
	UPROPERTY(BlueprintAssignable)
	FBlueprintOnSendInviteCompleteDelegate OnSuccess;

	// Called when failed to send invite
	UPROPERTY(BlueprintAssignable)
	FBlueprintOnSendInviteCompleteDelegate OnFailure;

	// Attempt to send invite
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Friends")
	static USendInviteCallbackProxy* SendInvite(UObject* WorldContextObject, class APlayerController* PlayerController, int32 LocalUserNum, FBlueprintOnlineFriend Friend, EBlueprintFriendsListType ListName);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	// Internal callback when SendInvite completes. Calls OnSuccess or OnFailure
	void OnSendInviteComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr);

	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	// SendInvite details
	int32 LocalUserNumber;
	FBlueprintOnlineFriend InvitedFriend;
	EBlueprintFriendsListType InviteFriendsListType;

	// The world context object in which this call is taking place
	UObject* WorldContextObject;
};
