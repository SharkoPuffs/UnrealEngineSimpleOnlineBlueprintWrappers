// Copyright 2021 SharkoPuffs. All Rights Reserved.


#include "LogoutUserNumberCallbackProxy.h"

ULogoutUserNumberCallbackProxy::ULogoutUserNumberCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnLogoutCompleteDelegate::CreateUObject(this, &ThisClass::OnLogoutComplete))
{
}

ULogoutUserNumberCallbackProxy* ULogoutUserNumberCallbackProxy::LogoutUserNumber(UObject* WorldContextObject, APlayerController* PlayerController, int32 PlayerNum)
{
	ULogoutUserNumberCallbackProxy* Proxy = NewObject<ULogoutUserNumberCallbackProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->PlayerNum = PlayerNum;
	return Proxy;
}

void ULogoutUserNumberCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Identity = Helper->GetIdentityInterface();
		if (Identity.IsValid())
		{
			DelegateHandle = Identity->AddOnLogoutCompleteDelegate_Handle(PlayerNum, Delegate);
			Identity->Logout(PlayerNum);
			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Online Subsystem does not support Identity"), ELogVerbosity::Warning);
		}
	}
	OnFailure.Broadcast();
}

void ULogoutUserNumberCallbackProxy::OnLogoutComplete(int32 LocalUserNum, bool bWasSuccessful)
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Identity = Helper->GetIdentityInterface();
		Identity->ClearOnLogoutCompleteDelegate_Handle(LocalUserNum, DelegateHandle);
	}
	if (bWasSuccessful)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}
