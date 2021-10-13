// Copyright 2021 SharkoPuffs. All Rights Reserved.


#include "FindSessionsPlusCallbackProxy.h"

UFindSessionsPlusCallbackProxy::UFindSessionsPlusCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete))
{
}

UFindSessionsPlusCallbackProxy* UFindSessionsPlusCallbackProxy::FindSessionsPlus(UObject* WorldContextObject, APlayerController* PlayerController, FUniqueNetIdRepl NetId, int32 MaxResults, bool bUseLAN, FBlueprintSearchSettings SearchSettings)
{
	UFindSessionsPlusCallbackProxy* Proxy = NewObject< UFindSessionsPlusCallbackProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->MaxResults = MaxResults;
	Proxy->bUseLAN = bUseLAN;
	Proxy->SearchSettings = SearchSettings;
	Proxy->NetId = NetId;
	return Proxy;
}

void UFindSessionsPlusCallbackProxy::Activate()
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Sessions = Helper->GetSessionInterface();
		if (Sessions.IsValid())
		{
			DelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(Delegate);
			SearchObject = MakeShareable(new FOnlineSessionSearch);
			SearchObject->bIsLanQuery = bUseLAN;
			SearchObject->MaxSearchResults = MaxResults;
			if (SearchSettings.bSearchDedicatedOnly)
			{
				SearchObject->QuerySettings.Set(SEARCH_DEDICATED_ONLY, SearchSettings.bSearchDedicatedOnly, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.bSearchEmptyServersOnly)
			{
				SearchObject->QuerySettings.Set(SEARCH_EMPTY_SERVERS_ONLY, SearchSettings.bSearchEmptyServersOnly, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.bSearchNonEmptyServersOnly)
			{
				SearchObject->QuerySettings.Set(SEARCH_NONEMPTY_SERVERS_ONLY, SearchSettings.bSearchNonEmptyServersOnly, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.bSearchSecureServersOnly)
			{
				SearchObject->QuerySettings.Set(SEARCH_SECURE_SERVERS_ONLY, SearchSettings.bSearchSecureServersOnly, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.bSearchPresence)
			{
				SearchObject->QuerySettings.Set(SEARCH_PRESENCE, SearchSettings.bSearchPresence, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.MinimumSlotsAvailable > 0)
			{
				SearchObject->QuerySettings.Set(SEARCH_MINSLOTSAVAILABLE, SearchSettings.MinimumSlotsAvailable, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.SearchExcludeUniqueIds != "")
			{
				SearchObject->QuerySettings.Set(SEARCH_EXCLUDE_UNIQUEIDS, SearchSettings.SearchExcludeUniqueIds, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.SearchUser != "")
			{
				SearchObject->QuerySettings.Set(SEARCH_USER, SearchSettings.SearchUser, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.SearchKeywords != "")
			{
				SearchObject->QuerySettings.Set(SEARCH_KEYWORDS, SearchSettings.SearchKeywords, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.SearchMatchmakingQue != "")
			{
				SearchObject->QuerySettings.Set(SEARCH_MATCHMAKING_QUEUE, SearchSettings.SearchMatchmakingQue, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.SearchXboxLiveHopperName != "")
			{
				SearchObject->QuerySettings.Set(SEARCH_XBOX_LIVE_HOPPER_NAME, SearchSettings.SearchXboxLiveHopperName, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.SearchXboxLiveSessionTemplateName != "")
			{
				SearchObject->QuerySettings.Set(SEARCH_XBOX_LIVE_SESSION_TEMPLATE_NAME, SearchSettings.SearchXboxLiveSessionTemplateName, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.SearchSwitchSelectionMethod != "")
			{
				SearchObject->QuerySettings.Set(SEARCH_SWITCH_SELECTION_METHOD, SearchSettings.SearchSwitchSelectionMethod, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.bSearchLobbies)
			{
				SearchObject->QuerySettings.Set(SEARCH_LOBBIES, SearchSettings.bSearchLobbies, EOnlineComparisonOp::Equals);
			}
			if (SearchSettings.SearchUserAttributeTeam != "")
			{
				SearchObject->QuerySettings.Set(SEARCH_USER_ATTRIBUTE_TEAM, SearchSettings.SearchUserAttributeTeam, EOnlineComparisonOp::Equals);
			}
			Sessions->FindSessions(*NetId, SearchObject.ToSharedRef());

			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Online Subsystem does not support sessions"), ELogVerbosity::Warning);
		}
	}
	TArray<FBlueprintSessionResult> Results;
	OnFailure.Broadcast(Results);
}

void UFindSessionsPlusCallbackProxy::OnFindSessionsComplete(bool bWasSuccessful)
{
	auto Helper = IOnlineSubsystem::Get();
	if (Helper)
	{
		auto Sessions = Helper->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(DelegateHandle);
		}
	}

	TArray<FBlueprintSessionResult> Results;

	if (bWasSuccessful && SearchObject.IsValid())
	{
		for (auto& Result : SearchObject->SearchResults)
		{
			FBlueprintSessionResult BPResult;
			BPResult.OnlineResult = Result;
			Results.Add(BPResult);
		}

		OnSuccess.Broadcast(Results);
	}
	else
	{
		OnFailure.Broadcast(Results);
	}
}
