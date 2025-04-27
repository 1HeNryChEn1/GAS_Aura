// CopyRight HenryChen


#include "Game/LoadScreenSaveGame.h"

FSavedMap ULoadScreenSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	for(const FSavedMap& Map : SavedMaps)
	{
		if(Map.MapAssetName == InMapName)
		{
			return Map;
		}
	}
	return {};
}

bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
	for(const FSavedMap& Map : SavedMaps)
	{
		if(Map.MapAssetName == InMapName)
		{
			return true;
		}
	}
	return false;
}