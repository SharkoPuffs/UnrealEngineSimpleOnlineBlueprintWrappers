// Copyright 2021 SharkoPuffs. All Rights Reserved.


#include "DestroyNamedSessionCallbackProxy.h"

UDestroyNamedSessionCallbackProxy::UDestroyNamedSessionCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete))
{
}

UDestroyNamedSessionCallbackProxy* UDestroyNamedSessionCallbackProxy::DestroyNamedSession(UObject* WorldContextObject, APlayerController* PlayerController, ESessionNamesType SessionType)
{
	UDestroyNamedSessionCallbackProxy* Proxy = NewObject<UDestroyNamedSessionCallbackProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->SessionType = SessionType;
	return Proxy;
}

void UDestroyNamedSessionCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto SessionInterface = Helper->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			DelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(Delegate);
			if (SessionInterface->DestroySession(ESessionNamesTypeToFName(SessionType)))
			{
				return;
			}
			else
			{
				OnFailure.Broadcast();
			}
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Online subsystem does not support Sessions"), ELogVerbosity::Warning);
		}
	}
	OnFailure.Broadcast();
}

void UDestroyNamedSessionCallbackProxy::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto SessionInterface = Helper->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(DelegateHandle);
		}
		if (bWasSuccessful)
		{
			OnSuccess.Broadcast();
			return;
		}
	}
	OnFailure.Broadcast();
}
