// Copyright 2021 SharkoPuffs. All Rights Reserved.


#include "JoinSessionPlusCallbackProxy.h"

UJoinSessionPlusCallbackProxy::UJoinSessionPlusCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCompleted))
{
}

UJoinSessionPlusCallbackProxy* UJoinSessionPlusCallbackProxy::JoinSessionPlus(UObject* WorldContextObject, APlayerController* PlayerController, ESessionNamesType SessionType, const FBlueprintSessionResult& SearchResult)
{
	UJoinSessionPlusCallbackProxy* Proxy = NewObject<UJoinSessionPlusCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->OnlineSearchResult = SearchResult.OnlineResult;
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->SessionType = SessionType;
	return Proxy;
}

void UJoinSessionPlusCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Sessions = Helper->GetSessionInterface();
		if (Sessions.IsValid())
		{
			DelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(Delegate);
			TSharedPtr<const FUniqueNetId> UniqueNetIdPtr = PlayerControllerWeakPtr->GetLocalPlayer()->GetPreferredUniqueNetId().GetUniqueNetId();
			Sessions->JoinSession(*UniqueNetIdPtr, ESessionNamesTypeToFName(SessionType), OnlineSearchResult);
			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Online Subsystem does not support Sessions"), ELogVerbosity::Warning);
		}
	}
	OnFailure.Broadcast();
}

void UJoinSessionPlusCallbackProxy::OnCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	auto Helper = IOnlineSubsystem::Get();
	if(Helper)
	{
		auto Sessions = Helper->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(DelegateHandle);
			if (Result == EOnJoinSessionCompleteResult::Type::Success)
			{
				FString ConnectString;
				if (Sessions->GetResolvedConnectString(SessionName, ConnectString))
				{
					if (PlayerControllerWeakPtr.IsValid())
					{
						UE_LOG_ONLINE_SESSION(Log, TEXT("Join session: traveling to %s"), *ConnectString);
						PlayerControllerWeakPtr->ClientTravel(ConnectString, TRAVEL_Absolute);
						OnSuccess.Broadcast();
						return;
					}
					else
					{
						UE_LOG_ONLINE_SESSION(Error, TEXT("PlayerController is Invalid"));
						OnFailure.Broadcast();
						return;
					}
				}
				else
				{
					UE_LOG_ONLINE_SESSION(Error, TEXT("Could not get resolved connect string"));
					OnFailure.Broadcast();
					return;
				}
			}
			else
			{
				FString ResultString;
				switch (Result)
				{
				case EOnJoinSessionCompleteResult::AlreadyInSession:
				{
					ResultString = "Already in Session";
				}
				case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
				{
					ResultString = "Could not retrieve address";
				}
				case EOnJoinSessionCompleteResult::SessionDoesNotExist:
				{
					ResultString = "Session does not exist";
				}
				case EOnJoinSessionCompleteResult::SessionIsFull:
				{
					ResultString = "Session is full";
				}
				case EOnJoinSessionCompleteResult::UnknownError:
				{
					ResultString = "Unknown error";
				}
				case EOnJoinSessionCompleteResult::Success:
				{
					ResultString = "Unknown";
				}
				}
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Join Session Failed with result: %s"), *ResultString);
			}
		}
		else
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Couldn't get Session Interface"));
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Couldn't get Online Subsystem"));
	}
	OnFailure.Broadcast();
}
