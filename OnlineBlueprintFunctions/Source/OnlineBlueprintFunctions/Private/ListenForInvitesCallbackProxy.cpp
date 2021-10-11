// Fill out your copyright notice in the Description page of Project Settings.


#include "ListenForInvitesCallbackProxy.h"

UListenForInvitesCallbackProxy* UListenForInvitesCallbackProxy::GetInviteReceiver(UObject* WorldContextObject, APlayerController* PlayerController)
{
	UListenForInvitesCallbackProxy* Proxy = NewObject<UListenForInvitesCallbackProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->PlayerControllerWeakPtr = PlayerController;
	return Proxy;
}

void UListenForInvitesCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	auto FriendInterface = Helper->GetFriendsInterface();
	OnReceivedInvitationDelegate_Handle = FriendInterface->AddOnInviteReceivedDelegate_Handle(FOnInviteReceivedDelegate::CreateUObject(this, &UListenForInvitesCallbackProxy::OnReceivedInvitation));
}

void UListenForInvitesCallbackProxy::OnReceivedInvitation(const FUniqueNetId& UserId, const FUniqueNetId& FriendId)
{
	FUniqueNetIdRepl UId = FUniqueNetIdRepl(UserId);
	FUniqueNetIdRepl FId = FUniqueNetIdRepl(FriendId);
	InviteReceived.Broadcast(UId, FId);
}