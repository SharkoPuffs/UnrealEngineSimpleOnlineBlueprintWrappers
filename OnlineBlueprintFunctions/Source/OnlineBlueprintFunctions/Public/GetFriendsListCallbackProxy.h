// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineBlueprintFunctionsTypes.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "GetFriendsListCallbackProxy.generated.h"

class APlayerController;

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API UGetFriendsListCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()
	
public:
	// Called when Friends list read successfully
	UPROPERTY(BlueprintAssignable)
	FBlueprintFriendsListDelegate OnSuccess;

	// Called when unable to read Friends list
	UPROPERTY(BlueprintAssignable)
	FBlueprintFriendsListDelegate OnFailure;

	// Attempt to get the players Friends list
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Friends")
	static UGetFriendsListCallbackProxy* GetFriendsList(UObject* WorldContextObject, class APlayerController* PlayerController, int32 LocalUserNum, EBlueprintFriendsListType FriendsListType);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:

	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	void OnFriendsListReadComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	int32 LocalUserNum;
	EBlueprintFriendsListType FriendsListType;
	UObject* WorldContextObject;
};
