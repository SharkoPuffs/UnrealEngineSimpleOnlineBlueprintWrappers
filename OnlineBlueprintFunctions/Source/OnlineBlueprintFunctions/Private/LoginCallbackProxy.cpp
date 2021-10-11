//

#include "LoginCallbackProxy.h"

/////////////////////////////////////////////////////////////
// ULoginCallbackProxy

ULoginCallbackProxy::ULoginCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, LoginCompleteDelegate(FOnLoginCompleteDelegate::CreateUObject(this, &ThisClass::OnLoginComplete))
	, Type("accountportal")
{
}

ULoginCallbackProxy* ULoginCallbackProxy::Login(UObject* WorldContextObject, class APlayerController* PlayerController, FString UserName, FString Password, FString Type, int32 PlayerNum)
{
	ULoginCallbackProxy* Proxy = NewObject<ULoginCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->UserName = UserName;
	Proxy->Password = Password;
	Proxy->Type = Type;
	Proxy->PlayerNum = PlayerNum;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void ULoginCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();

	if (Helper)
	{
		auto Identity = Helper->GetIdentityInterface();
		if (Identity.IsValid())
		{
			LoginCompleteDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(0, LoginCompleteDelegate);
			FOnlineAccountCredentials AccountCredentials;
			AccountCredentials.Id = UserName;
			AccountCredentials.Token = Password;
			AccountCredentials.Type = Type;

			Identity->Login(PlayerNum, AccountCredentials);

			// OnLoginComplete will get called when complete
			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Identity not supported by Online Subsystem"), ELogVerbosity::Warning);
		}
	}
	FUniqueNetIdRepl UId = FUniqueNetIdRepl();
	OnFailure.Broadcast(0, false, UId, "Login Failed");
}

void ULoginCallbackProxy::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrString)
{
	FUniqueNetIdRepl UId;
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Identity = Helper->GetIdentityInterface();
		if (Identity.IsValid())
		{
			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginCompleteDelegateHandle);
		}
		if (bWasSuccessful && PlayerControllerWeakPtr.IsValid())
		{
			UId = PlayerControllerWeakPtr->PlayerState->GetUniqueId();
			UId.SetUniqueNetId(FUniqueNetIdWrapper(UserId).GetUniqueNetId());
			PlayerControllerWeakPtr->PlayerState->SetUniqueId(UId);
			OnSuccess.Broadcast(LocalUserNum, bWasSuccessful, UId, ErrString);
		}
		else
		{
			UId = FUniqueNetIdRepl();
			OnFailure.Broadcast(LocalUserNum, bWasSuccessful, UId, ErrString);
		}
		return;
	}
	UId = FUniqueNetIdRepl();
	OnFailure.Broadcast(LocalUserNum, bWasSuccessful, UId, ErrString);
}
