// Copyright 2021 SharkoPuffs. All Rights Reserved.


#include "StartSessionCallbackProxy.h"

UStartSessionCallbackProxy::UStartSessionCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
}

UStartSessionCallbackProxy* UStartSessionCallbackProxy::StartSession(UObject* WorldContextObject, class APlayerController* PlayerController, ESessionNamesType SessionType)
{
	UStartSessionCallbackProxy* Proxy = NewObject<UStartSessionCallbackProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->SessionType = SessionType;
	return Proxy;
}

void UStartSessionCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto SessionInterface = Helper->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			DelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(Delegate);
			SessionInterface->StartSession(ESessionNamesTypeToFName(SessionType));
			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Online Subsystem does not support Sessions"), ELogVerbosity::Warning);
		}
	}
	OnFailure.Broadcast();
}

void UStartSessionCallbackProxy::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
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