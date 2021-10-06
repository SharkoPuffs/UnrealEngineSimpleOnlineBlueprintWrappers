// Fill out your copyright notice in the Description page of Project Settings.


#include "GetFriendsListCallbackProxy.h"

UGetFriendsListCallbackProxy::UGetFriendsListCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, OnReadFriendsListCompleteDelegate(FOnReadFriendsListComplete::CreateUObject(this, &ThisClass::OnFriendsListReadComplete))
{
}

UGetFriendsListCallbackProxy* UGetFriendsListCallbackProxy::GetFriendsList(UObject* WorldContextObject, APlayerController* PlayerController, int32 LocalUserNum, EBlueprintFriendsListType FriendsListType)
{
	UGetFriendsListCallbackProxy* Proxy = NewObject<UGetFriendsListCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->LocalUserNum = LocalUserNum;
	Proxy->FriendsListType = FriendsListType;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

const FString UGetFriendsListCallbackProxy::InviteStatusToString(EBlueprintInviteStatus EnumVal)
{
	switch (EnumVal)
	{
	case Unknown:
	{
		return TEXT("Unknown");
	}
	case Accepted:
	{
		return TEXT("Accepted");
	}
	case PendingInbound:
	{
		return TEXT("PendingInbound");
	}
	case PendingOutbound:
	{
		return TEXT("PendingOutbound");
	}
	case Blocked:
	{
		return TEXT("Blocked");
	}
	case Suggested:
	{
		return TEXT("Suggested");
	}
	}
	return TEXT("");
}

const FString UGetFriendsListCallbackProxy::FriendsListTypeToString(EBlueprintFriendsListType EnumVal)
{
	switch (EnumVal)
	{
	case Default:
	{
		return TEXT("Default");
	}
	case OnlinePlayers:
	{
		return TEXT("Online Players");
	}
	case InGamePlayers:
	{
		return TEXT("In Game Players");
	}
	case InGameAndSessionPlayers:
	{
		return TEXT("In Game and Session Players");
	}
	}
	return TEXT("");
}

EBlueprintInviteStatus UGetFriendsListCallbackProxy::GetInviteStatus(FBlueprintOnlineFriend Friend)
{
	return EBlueprintInviteStatus();
}

bool UGetFriendsListCallbackProxy::GetIsOnline(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().bIsOnline;
}

bool UGetFriendsListCallbackProxy::GetIsPlaying(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().bIsPlaying;
}

bool UGetFriendsListCallbackProxy::GetIsJoinable(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().bIsJoinable;
}

bool UGetFriendsListCallbackProxy::GetHasVoiceSupport(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().bHasVoiceSupport;
}

FDateTime UGetFriendsListCallbackProxy::GetLastOnline(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().LastOnline;
}

FString UGetFriendsListCallbackProxy::GetOnlineStatus(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().Status.StatusStr;
}

FString UGetFriendsListCallbackProxy::GetRealName(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetRealName();
}

FString UGetFriendsListCallbackProxy::GetDisplayName(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetDisplayName();
}

void UGetFriendsListCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	auto FriendsInterface = Helper->GetFriendsInterface();
	if (FriendsInterface.IsValid())
	{
		FriendsInterface->ReadFriendsList(LocalUserNum, FriendsListTypeToString(FriendsListType), OnReadFriendsListCompleteDelegate);
	}
	else
	{
		FFrame::KismetExecutionMessage(TEXT("Friends not supported by Online Subsystem"), ELogVerbosity::Warning);
	}
}

void UGetFriendsListCallbackProxy::OnFriendsListReadComplete(int32 LocalUserNumber, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	TArray<TSharedRef<FOnlineFriend, ESPMode::ThreadSafe>> Results;
	TArray<FBlueprintOnlineFriend> BlueprintResults;
	auto Helper = IOnlineSubsystem::Get();
	auto FriendsInterface = Helper->GetFriendsInterface();
	if (FriendsInterface.IsValid() && bWasSuccessful)
	{
		FriendsInterface->GetFriendsList(LocalUserNumber, ListName, Results);
		for (auto& Result : Results)
		{
			FBlueprintOnlineFriend FriendResult;
			FriendResult.OnlineFriend = &Result.Get();
			BlueprintResults.Add(FriendResult);
		}
		OnSuccess.Broadcast(BlueprintResults, ErrorStr);
	}
	else
	{
		OnFailure.Broadcast(BlueprintResults, ErrorStr);
	}
}
