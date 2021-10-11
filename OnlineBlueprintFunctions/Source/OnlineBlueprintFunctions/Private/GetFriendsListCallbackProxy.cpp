// Fill out your copyright notice in the Description page of Project Settings.


#include "GetFriendsListCallbackProxy.h"

UGetFriendsListCallbackProxy* UGetFriendsListCallbackProxy::GetFriendsList(UObject* WorldContextObject, APlayerController* PlayerController, int32 LocalUserNum, EBlueprintFriendsListType FriendsListType)
{
	UGetFriendsListCallbackProxy* Proxy = NewObject<UGetFriendsListCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->LocalUserNum = LocalUserNum;
	Proxy->FriendsListType = FriendsListType;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UGetFriendsListCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	auto FriendsInterface = Helper->GetFriendsInterface();
	if (FriendsInterface.IsValid())
	{
		FriendsInterface->ReadFriendsList(LocalUserNum, UOnlineBlueprintFunctionsTypes::FriendsListTypeToString(FriendsListType), FOnReadFriendsListComplete::CreateUObject(this, &ThisClass::OnFriendsListReadComplete));
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
