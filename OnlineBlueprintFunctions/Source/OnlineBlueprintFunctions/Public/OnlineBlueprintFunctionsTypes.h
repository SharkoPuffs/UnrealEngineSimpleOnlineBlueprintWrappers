// Copyright 2021 SharkoPuffs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlinePresenceInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
#include "Online/CoreOnline.h"
#include "GameFramework/OnlineReplStructs.h"
#include "OnlineBlueprintFunctionsTypes.generated.h"

USTRUCT(BlueprintType)
struct FOnlineSessionSettingsHolder
{
	GENERATED_BODY()

	FOnlineSessionSettings Settings;

	FOnlineSessionSettingsHolder()
	{
		Settings = FOnlineSessionSettings();
	}
};

USTRUCT(BlueprintType)
struct FBlueprintSessionSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		int32 NumberOfPublicConnections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		int32 NumberOfPrivateConnections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bShouldAdvertiseSession;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bAllowJoinGameInProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bUseLAN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bIsDedicatedServer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bUseStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bEnableInvites;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bUsePresence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bAllowJoinUsingPresence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bAllowJoinUsingPresenceFriendsOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bAntiCheat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bUseLobbiesIfSupported;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		bool bUseLobbiesChatIfSupported;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		int32 UniqueId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
		FString SearchKeywords;

	FBlueprintSessionSettings()
	{
		NumberOfPublicConnections = 1;
		NumberOfPrivateConnections = 1;
		bShouldAdvertiseSession = false;
		bAllowJoinGameInProgress = false;
		bUseLAN = false;
		bIsDedicatedServer = false;
		bUseStats = false;
		bEnableInvites = false;
		bUsePresence = false;
		bAllowJoinUsingPresence = false;
		bAllowJoinUsingPresenceFriendsOnly = false;
		bAntiCheat = false;
		bUseLobbiesIfSupported = false;
		bUseLobbiesChatIfSupported = false;
		UniqueId = 0;
		SearchKeywords = "";
	}
};

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

UENUM(BlueprintType)
enum EBlueprintSessionState
{
	NoSession UMETA(DisplayName = "No Session"),

	Creating UMETA(DisplayName = "Creating"),

	Pending UMETA(DisplayName = "Pending"),

	Starting UMETA(DisplayName = "Starting"),

	InProgress UMETA(DisplayName = "In Progress"),

	Ending UMETA(DisplayName = "Ending"),

	Ended UMETA(DisplayName = "Ended"),

	Destroying UMETA(DisplayName = "Destroying")
};

USTRUCT(BlueprintType)
struct FBlueprintOnlineFriend
{
	GENERATED_USTRUCT_BODY()

	FOnlineFriend* OnlineFriend;
	FBlueprintOnlineFriend()
	{
		OnlineFriend = nullptr;
	};
};

USTRUCT(BlueprintType)
struct FBlueprintSearchSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	bool bSearchDedicatedOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	bool bSearchEmptyServersOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	bool bSearchNonEmptyServersOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	bool bSearchSecureServersOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	bool bSearchPresence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	int32 MinimumSlotsAvailable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	FString SearchExcludeUniqueIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	FString SearchUser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	FString SearchKeywords;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	FString SearchMatchmakingQue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	FString SearchXboxLiveHopperName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	FString SearchXboxLiveSessionTemplateName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	FString SearchSwitchSelectionMethod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	bool bSearchLobbies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online|Session")
	FString SearchUserAttributeTeam;

	FBlueprintSearchSettings()
	{
		bSearchDedicatedOnly = false;
		bSearchEmptyServersOnly = false;
		bSearchNonEmptyServersOnly = false;
		bSearchSecureServersOnly = false;
		bSearchPresence = false;
		MinimumSlotsAvailable = 0;
		SearchExcludeUniqueIds = "";
		SearchUser = "";
		SearchKeywords = "";
		SearchMatchmakingQue = "";
		SearchXboxLiveHopperName = "";
		SearchXboxLiveSessionTemplateName = "";
		SearchSwitchSelectionMethod = "";
		bSearchLobbies = false;
		SearchUserAttributeTeam = "";
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBlueprintFriendsListDelegate, const TArray<FBlueprintOnlineFriend>&, FriendsListResults, const FString&, ErrStr);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBlueprintLoginCompleteDelegate, int32, LocalUserNum, bool, bWasSuccessful, FUniqueNetIdRepl, UserId, const FString&, ErrString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBlueprintOnReceivedInvite, FUniqueNetIdRepl, UserId, FUniqueNetIdRepl, FriendId);

UCLASS()
class ONLINEBLUEPRINTFUNCTIONS_API UOnlineBlueprintFunctionsTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities|Struct")
	static FBlueprintSessionSettings BreakOnlineSessionSettings(FOnlineSessionSettingsHolder Settings);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utilities|Struct")
	static FOnlineSessionSettingsHolder MakeOnlineSessionSettings(FBlueprintSessionSettings Settings);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static const FString InviteStatusToString(EBlueprintInviteStatus EnumVal);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static const FString FriendsListTypeToString(EBlueprintFriendsListType EnumVal);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static EBlueprintInviteStatus GetInviteStatus(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static bool GetIsOnline(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static bool GetIsPlaying(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static bool GetIsJoinable(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static bool GetHasVoiceSupport(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static FDateTime GetLastOnline(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static FString GetOnlineStatus(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static FString GetRealName(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Friends")
		static FString GetDisplayName(FBlueprintOnlineFriend Friend);

	UFUNCTION(BlueprintCallable, Category = "Online|Identity")
		static FUniqueNetIdRepl GetNetId(int32 LocalUserNum);

	UFUNCTION(BlueprintCallable, Category = "Online|Identity")
		static bool IsPlayerOnline(int32 LocalUserNum);

	UFUNCTION(BlueprintCallable, Category = "Online|Session")
		static EBlueprintSessionState GetSessionState(FName SessionName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Identity")
		static FString GetLocalPlayerDisplayName(int32 LocalUserNum);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Online|Identity")
		static FString GetOnlinePlayerDisplayName(FUniqueNetIdRepl UniqueId);
};

UENUM(BlueprintType)
	enum ESessionNamesType
{
	GameSession UMETA(DisplayName = "Game Session"),
	PartySession UMETA(DisplayName = "PartySession")
};

static inline FName ESessionNamesTypeToFName(ESessionNamesType SessionType)
{
	switch (SessionType)
	{
	case ESessionNamesType::GameSession:
		return NAME_GameSession;
	case ESessionNamesType::PartySession:
		return NAME_PartySession;
	}
	return NAME_GameSession;
};
