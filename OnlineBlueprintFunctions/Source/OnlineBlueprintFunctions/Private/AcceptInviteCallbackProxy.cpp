// Fill out your copyright notice in the Description page of Project Settings.


#include "AcceptInviteCallbackProxy.h"

UAcceptInviteCallbackProxy* UAcceptInviteCallbackProxy::AcceptInvite(UObject* WorldContextObject, APlayerController* PlayerController, int32 LocalUserNum, FBlueprintOnlineFriend Friend, EBlueprintFriendsListType ListName)
{
	UAcceptInviteCallbackProxy* Proxy = NewObject<UAcceptInviteCallbackProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->LocalUserNumber = LocalUserNum;
	Proxy->Friend = Friend;
	Proxy->ListName = ListName;
	return Proxy;
}

void UAcceptInviteCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto FriendInterface = Helper->GetFriendsInterface();
		if (FriendInterface.IsValid())
		{
			FriendInterface->AcceptInvite(LocalUserNumber, Friend.OnlineFriend->GetUserId().Get(), UOnlineBlueprintFunctionsTypes::FriendsListTypeToString(ListName), FOnAcceptInviteComplete::CreateUObject(this, &UAcceptInviteCallbackProxy::OnAcceptInviteComplete));
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Online Subsystem does not support friends"), ELogVerbosity::Warning);
		}
	}
	else
	{
		OnFailure.Broadcast(FString("Unable to get Online Subsystem"));
	}
}

void UAcceptInviteCallbackProxy::OnAcceptInviteComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& FriendListName, const FString& ErrorStr)
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
