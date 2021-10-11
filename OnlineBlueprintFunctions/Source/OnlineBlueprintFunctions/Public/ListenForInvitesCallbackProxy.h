// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "OnlineBlueprintFunctionsTypes.h"
#include "ListenForInvitesCallbackProxy.generated.h"

class APlayerController;

UCLASS(Blueprintable)
class ONLINEBLUEPRINTFUNCTIONS_API UListenForInvitesCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Online|Friends")
	FBlueprintOnReceivedInvite InviteReceived;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "Online|Friends")
	static UListenForInvitesCallbackProxy* GetInviteReceiver(UObject* WorldContextObject, class APlayerController* PlayerController);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	void OnReceivedInvitation(const FUniqueNetId& UserId, const FUniqueNetId& FriendId);

	FDelegateHandle OnReceivedInvitationDelegate_Handle;
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;
	UObject* WorldContextObject;
};
