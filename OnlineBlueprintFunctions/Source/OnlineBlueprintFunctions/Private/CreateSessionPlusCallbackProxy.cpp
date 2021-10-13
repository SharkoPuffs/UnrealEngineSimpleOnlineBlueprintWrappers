// Copyright 2021 SharkoPuffs. All Rights Reserved.


#include "CreateSessionPlusCallbackProxy.h"
#include "Engine/NetDriver.h"

UCreateSessionPlusCallbackProxy::UCreateSessionPlusCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
{
}

UCreateSessionPlusCallbackProxy* UCreateSessionPlusCallbackProxy::CreateSessionPlus(UObject* WorldContextObject, APlayerController* PlayerController, FUniqueNetIdRepl NetId, ESessionNamesType SessionType, FBlueprintSessionSettings Settings)
{
	UCreateSessionPlusCallbackProxy* Proxy = NewObject<UCreateSessionPlusCallbackProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->SessionType = SessionType;
	Proxy->Settings = Settings;
	Proxy->NetId = NetId;
	return Proxy;
}

void UCreateSessionPlusCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto SessionInterface = Helper->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			if (NetId.IsValid())
			{
				DelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(Delegate);
				FOnlineSessionSettings NewSettings = Settings.GetSettings();
				SessionInterface->CreateSession(*NetId, ESessionNamesTypeToFName(SessionType), NewSettings);
				return;
			}
			else
			{
				FFrame::KismetExecutionMessage(TEXT("Unique Id is Invalid"), ELogVerbosity::Warning);
			}
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Online Subsystem does not support Sessions"), ELogVerbosity::Warning);
		}
	}
	OnFailure.Broadcast();
}

void UCreateSessionPlusCallbackProxy::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto SessionInterface = Helper->GetSessionInterface();
		if (SessionInterface.IsValid() && bWasSuccessful)
		{
			SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(DelegateHandle);
			OnSuccess.Broadcast();
			return;
		}
	}
	OnFailure.Broadcast();
}
