// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "GetFriendsListCallbackProxy.generated.h"

class APlayerController;

UENUM(BlueprintType)
enum EBlueprintInviteStatus
{
	Unknown UMETA(DisplayName = "Unknown"),

	Accepted UMETA(DisplayName = "Accepted"),

	PendingInbound UMETA(DisplayName = "Pending Inbound"),

	PendingOutbound UMETA(DisplayName = "Pending Outbound"),

	Blocked UMETA(DisplayName = "Blocked"),

	Suggested UMETA(DisplayName = "Suggested")
};

UENUM(BlueprintType)
enum EBlueprintFriendsListType
{
	Default UMETA(DisplayName = "Default"),
	OnlinePlayers UMETA(DisplayName = "Online Players"),
	InGamePlayers UMETA(DisplayName = "In Game Players"),
	InGameAndSessionPlayers UMETA(DisplayName = "In Game And Session Players")
};

USTRUCT(BlueprintType)
struct FBlueprintOnlineFriend
{
	GENERATED_USTRUCT_BODY()

	FOnlineFriend* OnlineFriend;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBlueprintFriendsListDelegate, const TArray<FBlueprintOnlineFriend>&, FriendsListResults, const FString&, ErrStr);

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API UGetFriendsListCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()
	
	// Called when Friends list read successfully
	UPROPERTY(BlueprintAssignable)
	FBlueprintFriendsListDelegate OnSuccess;

	// Called when unable to read Friends list
	UPROPERTY(BlueprintAssignable)
	FBlueprintFriendsListDelegate OnFailure;

	// Attempt to get the players Friends list
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Online|Friends")
	static UGetFriendsListCallbackProxy* GetFriendsList(UObject* WorldContextObject, class APlayerController* PlayerController, int32 LocalUserNum, EBlueprintFriendsListType FriendsListType);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static const FString InviteStatusToString(EBlueprintInviteStatus EnumVal);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static const FString FriendsListTypeToString(EBlueprintFriendsListType EnumVal);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static EBlueprintInviteStatus GetInviteStatus(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static bool GetIsOnline(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static bool GetIsPlaying(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static bool GetIsJoinable(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static bool GetHasVoiceSupport(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static FDateTime GetLastOnline(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static FString GetOnlineStatus(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static FString GetRealName(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintPure, Category = "Online|Friends")
	static FString GetDisplayName(FBlueprintOnlineFriend Friend);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:

	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	FOnReadFriendsListComplete OnReadFriendsListCompleteDelegate;

	void OnFriendsListReadComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	int32 LocalUserNum;
	EBlueprintFriendsListType FriendsListType;
	UObject* WorldContextObject;
};
