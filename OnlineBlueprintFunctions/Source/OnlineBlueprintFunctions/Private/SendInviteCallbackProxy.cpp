// Copyright 2021 SharkoPuffs. All Rights Reserved.

#include "SendInviteCallbackProxy.h"

/////////////////////////////////////////////////////////////
// USendInviteCallbackProxy

USendInviteCallbackProxy* USendInviteCallbackProxy::SendInvite(UObject* WorldContextObject, APlayerController* PlayerController, int32 LocalUserNum, FBlueprintOnlineFriend Friend, EBlueprintFriendsListType ListName)
{
	USendInviteCallbackProxy* Proxy = NewObject<USendInviteCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->LocalUserNumber = LocalUserNum;
	Proxy->InvitedFriend = Friend;
	Proxy->InviteFriendsListType = ListName;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void USendInviteCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto FriendInterface = Helper->GetFriendsInterface();
		if (FriendInterface.IsValid())
		{
			if (!FriendInterface->SendInvite(LocalUserNumber, InvitedFriend.OnlineFriend->GetUserId().Get(), UOnlineBlueprintFunctionsTypes::FriendsListTypeToString(InviteFriendsListType), FOnSendInviteComplete::CreateUObject(this, &ThisClass::OnSendInviteComplete)))
			{
				OnFailure.Broadcast(FString("Failed to send invite"));
				return;
			}
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Online Subsystem does not support Friends"), ELogVerbosity::Warning);
		}
	}
	OnFailure.Broadcast(FString("Failed to get OnlineSubsystem"));
}

void USendInviteCallbackProxy::OnSendInviteComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr)
{
	if (bWasSuccessful)
	{
		OnSuccess.Broadcast(ErrorStr);
	}
	else
	{
		OnFailure.Broadcast(ErrorStr);
	}
}
