// Copyright 2021 SharkoPuffs. All Rights Reserved.

#include "OnlineBlueprintFunctionsTypes.h"

FBlueprintSessionSettings UOnlineBlueprintFunctionsTypes::BreakOnlineSessionSettings(FOnlineSessionSettingsHolder Settings)
{
	FBlueprintSessionSettings NewSettings = FBlueprintSessionSettings();
	NewSettings.bAllowJoinGameInProgress = Settings.Settings.bAllowJoinInProgress;
	NewSettings.bAllowJoinUsingPresence = Settings.Settings.bAllowJoinViaPresence;
	NewSettings.bAllowJoinUsingPresenceFriendsOnly = Settings.Settings.bAllowJoinViaPresenceFriendsOnly;
	NewSettings.bAntiCheat = Settings.Settings.bAntiCheatProtected;
	NewSettings.bEnableInvites = Settings.Settings.bAllowInvites;
	NewSettings.bIsDedicatedServer = Settings.Settings.bIsDedicated;
	NewSettings.bShouldAdvertiseSession = Settings.Settings.bShouldAdvertise;
	NewSettings.bUseLAN = Settings.Settings.bIsLANMatch;
	NewSettings.bUseLobbiesChatIfSupported = Settings.Settings.bUseLobbiesVoiceChatIfAvailable;
	NewSettings.bUseLobbiesIfSupported = Settings.Settings.bUseLobbiesIfAvailable;
	NewSettings.bUsePresence = Settings.Settings.bUsesPresence;
	NewSettings.bUseStats = Settings.Settings.bUsesStats;
	NewSettings.NumberOfPrivateConnections = Settings.Settings.NumPrivateConnections;
	NewSettings.NumberOfPublicConnections = Settings.Settings.NumPublicConnections;
	NewSettings.UniqueId = Settings.Settings.BuildUniqueId;
	FString Keywords;
	Settings.Settings.Get(SEARCH_KEYWORDS, Keywords);
	NewSettings.SearchKeywords = Keywords;
	return NewSettings;
}

FOnlineSessionSettingsHolder UOnlineBlueprintFunctionsTypes::MakeOnlineSessionSettings(FBlueprintSessionSettings Settings)
{
	FOnlineSessionSettingsHolder NewSettings = FOnlineSessionSettingsHolder();
	NewSettings.Settings.bAllowInvites = Settings.bEnableInvites;
	NewSettings.Settings.bAllowJoinInProgress = Settings.bAllowJoinGameInProgress;
	NewSettings.Settings.bAllowJoinViaPresence = Settings.bAllowJoinUsingPresence;
	NewSettings.Settings.bAllowJoinViaPresenceFriendsOnly = Settings.bAllowJoinUsingPresenceFriendsOnly;
	NewSettings.Settings.bAntiCheatProtected = Settings.bAntiCheat;
	NewSettings.Settings.bIsDedicated = Settings.bIsDedicatedServer;
	NewSettings.Settings.bIsLANMatch = Settings.bUseLAN;
	NewSettings.Settings.bShouldAdvertise = Settings.bShouldAdvertiseSession;
	if (Settings.UniqueId != 0)
	{
		NewSettings.Settings.BuildUniqueId = Settings.UniqueId;
	}
	NewSettings.Settings.bUseLobbiesIfAvailable = Settings.bUseLobbiesIfSupported;
	NewSettings.Settings.bUseLobbiesVoiceChatIfAvailable = Settings.bUseLobbiesChatIfSupported;
	NewSettings.Settings.bUsesPresence = Settings.bUsePresence;
	NewSettings.Settings.bUsesStats = Settings.bUseStats;
	NewSettings.Settings.NumPrivateConnections = Settings.NumberOfPrivateConnections;
	NewSettings.Settings.NumPublicConnections = Settings.NumberOfPublicConnections;
	NewSettings.Settings.Set(SEARCH_KEYWORDS, Settings.SearchKeywords);
	return NewSettings;
}

const FString UOnlineBlueprintFunctionsTypes::InviteStatusToString(EBlueprintInviteStatus EnumVal)
{
	switch (EnumVal)
	{
	case Unknown:
	{
		return TEXT("Unknown");
	}
	case Accepted:
	{
		return TEXT("Accepted");
	}
	case PendingInbound:
	{
		return TEXT("PendingInbound");
	}
	case PendingOutbound:
	{
		return TEXT("PendingOutbound");
	}
	case Blocked:
	{
		return TEXT("Blocked");
	}
	case Suggested:
	{
		return TEXT("Suggested");
	}
	}
	return TEXT("");
}

const FString UOnlineBlueprintFunctionsTypes::FriendsListTypeToString(EBlueprintFriendsListType EnumVal)
{
	switch (EnumVal)
	{
	case Default:
	{
		return TEXT("Default");
	}
	case OnlinePlayers:
	{
		return TEXT("Online Players");
	}
	case InGamePlayers:
	{
		return TEXT("In Game Players");
	}
	case InGameAndSessionPlayers:
	{
		return TEXT("In Game and Session Players");
	}
	}
	return TEXT("");
}

EBlueprintInviteStatus UOnlineBlueprintFunctionsTypes::GetInviteStatus(FBlueprintOnlineFriend Friend)
{
	EInviteStatus::Type EnumVal = Friend.OnlineFriend->GetInviteStatus();
	switch (EnumVal)
	{
	case Unknown:
	{
		return EBlueprintInviteStatus::Unknown;
	}
	case Accepted:
	{
		return EBlueprintInviteStatus::Accepted;
	}
	case PendingInbound:
	{
		return EBlueprintInviteStatus::PendingInbound;
	}
	case PendingOutbound:
	{
		return EBlueprintInviteStatus::PendingOutbound;
	}
	case Blocked:
	{
		return EBlueprintInviteStatus::Blocked;
	}
	case Suggested:
	{
		return EBlueprintInviteStatus::Suggested;
	}
	}
	return EBlueprintInviteStatus::Unknown;
}

bool UOnlineBlueprintFunctionsTypes::GetIsOnline(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().bIsOnline;
}

bool UOnlineBlueprintFunctionsTypes::GetIsPlaying(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().bIsPlaying;
}

bool UOnlineBlueprintFunctionsTypes::GetIsJoinable(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().bIsJoinable;
}

bool UOnlineBlueprintFunctionsTypes::GetHasVoiceSupport(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().bHasVoiceSupport;
}

FDateTime UOnlineBlueprintFunctionsTypes::GetLastOnline(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().LastOnline;
}

FString UOnlineBlueprintFunctionsTypes::GetOnlineStatus(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetPresence().Status.StatusStr;
}

FString UOnlineBlueprintFunctionsTypes::GetRealName(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetRealName();
}

FString UOnlineBlueprintFunctionsTypes::GetDisplayName(FBlueprintOnlineFriend Friend)
{
	return Friend.OnlineFriend->GetDisplayName();
}

FUniqueNetIdRepl UOnlineBlueprintFunctionsTypes::GetNetId(int32 LocalUserNum)
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Identity = Helper->GetIdentityInterface();
		if (Identity.IsValid())
		{
			return FUniqueNetIdRepl(Identity->GetUniquePlayerId(LocalUserNum));
		}
	}
	return FUniqueNetIdRepl();
}

bool UOnlineBlueprintFunctionsTypes::IsPlayerOnline(int32 LocalUserNum)
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Identity = Helper->GetIdentityInterface();
		if (Identity.IsValid())
		{
			return Identity->GetLoginStatus(LocalUserNum) == ELoginStatus::Type::LoggedIn;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Online Subsystem does not support Identity"), ELogVerbosity::Warning);
		}
	}
	return false;
}

EBlueprintSessionState UOnlineBlueprintFunctionsTypes::GetSessionState(FName SessionName)
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Session = Helper->GetSessionInterface();
		if (Session.IsValid())
		{
			switch (Session->GetSessionState(SessionName))
			{
			case EOnlineSessionState::Creating:
			{
				return EBlueprintSessionState::Creating;
			}
			case EOnlineSessionState::Destroying:
			{
				return EBlueprintSessionState::Destroying;
			}
			case EOnlineSessionState::Ended:
			{
				return EBlueprintSessionState::Ended;
			}
			case EOnlineSessionState::Ending:
			{
				return EBlueprintSessionState::Ending;
			}
			case EOnlineSessionState::InProgress:
			{
				return EBlueprintSessionState::InProgress;
			}
			case EOnlineSessionState::NoSession:
			{
				return EBlueprintSessionState::NoSession;
			}
			case EOnlineSessionState::Pending:
			{
				return EBlueprintSessionState::Pending;
			}
			case EOnlineSessionState::Starting:
			{
				return EBlueprintSessionState::Starting;
			}
			}
		}
	}
	return EBlueprintSessionState();
}

FString UOnlineBlueprintFunctionsTypes::GetLocalPlayerDisplayName(int32 LocalUserNum)
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Identity = Helper->GetIdentityInterface();
		if (Identity.IsValid())
		{
			return Identity->GetUserAccount(*Identity->GetUniquePlayerId(LocalUserNum).Get())->GetDisplayName();
		}
	}
	return FString();
}

FString UOnlineBlueprintFunctionsTypes::GetOnlinePlayerDisplayName(FUniqueNetIdRepl UniqueId)
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Identity = Helper->GetIdentityInterface();
		if (Identity.IsValid())
		{
			if (UniqueId.IsValid())
			{
				const FUniqueNetId* UId = UniqueId.GetUniqueNetId().Get();
				return Identity->GetUserAccount(*UId)->GetDisplayName();
			}
		}
	}
	return FString("Player");
}
