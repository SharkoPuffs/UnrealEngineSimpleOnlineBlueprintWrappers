// Copyright 2021 SharkoPuffs. All Rights Reserved.


#include "CreateSessionPlusCallbackProxy.h"
#include "Engine/NetDriver.h"

UCreateSessionPlusCallbackProxy::UCreateSessionPlusCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
{
}

UCreateSessionPlusCallbackProxy* UCreateSessionPlusCallbackProxy::CreateSessionPlus(UObject* WorldContextObject, APlayerController* PlayerController, ESessionNamesType SessionType, FBlueprintSessionSettings Settings)
{
	UCreateSessionPlusCallbackProxy* Proxy = NewObject<UCreateSessionPlusCallbackProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->SessionType = SessionType;
	Proxy->Settings = Settings;
	return Proxy;
}

void UCreateSessionPlusCallbackProxy::Activate()
{
	if (PlayerControllerWeakPtr.IsValid())
	{
		auto Helper = IOnlineSubsystem::Get();
		if (Helper)
		{
			auto SessionInterface = Helper->GetSessionInterface();
			if (SessionInterface.IsValid())
			{
				DelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(Delegate);
				TSharedPtr<const FUniqueNetId> UniqueNetIdPtr = PlayerControllerWeakPtr->GetLocalPlayer()->GetPreferredUniqueNetId().GetUniqueNetId();
				SessionInterface->CreateSession(*UniqueNetIdPtr, ESessionNamesTypeToFName(SessionType), UOnlineBlueprintFunctionsTypes::MakeOnlineSessionSettings(Settings).Settings);
				return;
			}
			else
			{
				FFrame::KismetExecutionMessage(TEXT("Online Subsystem does not support Sessions"), ELogVerbosity::Warning);
			}
		}
		OnFailure.Broadcast();
		return;
	}
	UE_LOG_ONLINE_SESSION(Warning, TEXT("PlayerController is invalid!"))
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
