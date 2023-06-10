#include "STDInc.hpp"

namespace Components
{
	std::recursive_mutex LocalizedStrings::LocalizeMutex;
	std::unordered_map<std::string, Game::LocalizeEntry*> LocalizedStrings::LocalizeMap;

	std::optional<std::string> LocalizedStrings::PrefixOverride;
	std::function<void(Game::LocalizeEntry*)> LocalizedStrings::ParseCallback;

	void LocalizedStrings::OverrideLocalizeStrings()
	{
		// Clearing previous localizeMap objects when game has been initialized or restarted.
		// Otherwise, game will be replacing the localize entries even if you have you have a another localization.
		LocalizeMap.clear();

		const char* languageName = Game::SEH_GetLanguageName(Dvars::Functions::Dvar_FindVar("loc_language")->current.unsignedInt);

		if (!strcmp(languageName, "english"))
		{
			LocalizedStrings::Set("MENU_LAUNCH_WITHOUT_MODS", "Launch without Mods");
			LocalizedStrings::Set("SUBTITLE_AMBUSH_GAZ_DROPGUN82", "^2Gaz: ^7Drop the bloody gun! Now, drop it!");
			LocalizedStrings::Set("SUBTITLE_BLACKOUT_PRI_SMELLTHATGAZ231", "^2Captain Price: ^7Gaz, you smell that?");
			LocalizedStrings::Set("SUBTITLE_BLACKOUT_PRI_CUTPOWER61", "^2Captain Price: ^7Gaz, go around the back and cut the power. Soap, get ready!");
			LocalizedStrings::Set("SUBTITLE_BLACKOUT_PRI_LETSGO81", "^2Captain Price: ^7Let's go!");
			LocalizedStrings::Set("SUBTITLE_KILLHOUSE_GAZ_RIFLEDOWNRANGE32", "^2Gaz: ^7Now aim your rifle down range.");
			LocalizedStrings::Set("KILLHOUSE_ACCURACY_BONUS", "Accuracy bonus: -");
			LocalizedStrings::Set("KILLHOUSE_ACCURACY_BONUS_ZERO", "Accuracy bonus: -0.0");
			return;
		}
		else if (!strcmp(languageName, "german"))
		{
			LocalizedStrings::Set("MENU_RUSSIAN", "Russisch");
			LocalizedStrings::Set("KILLHOUSE_ACCURACY_BONUS", "Präzisionsbonus: -");
			LocalizedStrings::Set("KILLHOUSE_ACCURACY_BONUS_ZERO", "Präzisionsbonus: -0.0");
			return;
		}
		else if (!strcmp(languageName, "russian"))
		{
			LocalizedStrings::Set("MENU_ENGLISH", "Àíãëèéñêèé");
			return;
		}
		else
			return;
	}

	void LocalizedStrings::Set(const std::string& psLocalReference, const std::string& psNewString)
	{
		std::lock_guard _(LocalizeMutex);
		Utils::Memory::Allocator* allocator = Utils::Memory::GetAllocator();

		auto key = psLocalReference;
		if (PrefixOverride.has_value())
		{
			key.insert(0, PrefixOverride.value());
		}

		if (LocalizeMap.contains(key))
		{
			auto* entry = LocalizeMap[key];

			const auto* newStaticValue = allocator->duplicateString(psNewString);
			if (!newStaticValue) return;

			if (entry->value) allocator->free(entry->value);
			entry->value = newStaticValue;

			LocalizedStrings::SaveParseOutput(entry);

			return;
		}

		auto* entry = allocator->allocate<Game::LocalizeEntry>();
		if (!entry) return;

		entry->name = allocator->duplicateString(key);
		if (!entry->name)
		{
			allocator->free(entry);
			return;
		}

		entry->value = allocator->duplicateString(psNewString);
		if (!entry->value)
		{
			allocator->free(entry->name);
			allocator->free(entry);
			return;
		}

		LocalizedStrings::SaveParseOutput(entry);

		LocalizeMap[key] = entry;
	}

	void LocalizedStrings::SaveParseOutput(Game::LocalizeEntry* asset)
	{
		if (ParseCallback)
		{
			ParseCallback(asset);
		}
	}

	const char* LocalizedStrings::Get(const char* key)
	{
		Game::LocalizeEntry* entry = nullptr;
		{
			std::lock_guard _(LocalizeMutex);

			if (LocalizeMap.contains(key))
			{
				entry = LocalizeMap[key];
			}
		}

		if (!entry || !entry->value)
		{
			entry = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_LOCALIZE_ENTRY, key).localize;
		}

		if (entry && entry->value)
		{
			return entry->value;
		}

		return key;
	}

	void __stdcall LocalizedStrings::SetStringStub(const char* psLocalReference, const char* psNewString, [[maybe_unused]] int bSentenceIsEnglish)
	{
		LocalizedStrings::Set(psLocalReference, psNewString);
	}

	LocalizedStrings::LocalizedStrings()
	{
		AssetHandler::OnFind(Game::XAssetType::ASSET_TYPE_LOCALIZE_ENTRY, [](Game::XAssetType, const std::string& name)
		{
			Game::XAssetHeader header = { nullptr };
			std::lock_guard _(LocalizeMutex);

			if (const auto itr = LocalizeMap.find(name); itr != LocalizeMap.end())
			{
				header.localize = itr->second;
			}

			return header;
		});

		// Resolving hook
		Utils::Hook(0x55BEA0, LocalizedStrings::Get, HOOK_JUMP).install()->quick();
		Utils::Hook(0x55CFE9, LocalizedStrings::SetStringStub, HOOK_CALL).install()->quick();
	}

	LocalizedStrings::~LocalizedStrings()
	{
		LocalizeMap.clear();
	}
}
