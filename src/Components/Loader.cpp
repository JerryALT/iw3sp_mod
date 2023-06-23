#include "STDInc.hpp"

namespace Components
{
	bool Loader::Pregame = true;
	bool Loader::Postgame = false;
	bool Loader::Uninitializing = false;
	std::vector<Component*> Loader::Components;

	bool Loader::IsPregame()
	{
		return Loader::Pregame;
	}

	bool Loader::IsPostgame()
	{
		return Loader::Postgame;
	}

	bool Loader::IsUninitializing()
	{
		return Loader::Uninitializing;
	}

	void Loader::Initialize()
	{
		Loader::Pregame = true;
		Loader::Postgame = false;
		Loader::Uninitializing = false;

		Loader::Register(new Config());
		Loader::Register(new Flags());
		Loader::Register(new FastFiles());
		Loader::Register(new Command());
		Loader::Register(new AssetHandler());
		Loader::Register(new Branding());
		Loader::Register(new FileSystem());
		Loader::Register(new Language());
		Loader::Register(new CommonPatch());
		Loader::Register(new Maps());
		Loader::Register(new D3D9Ex());
		Loader::Register(new Renderer());
		Loader::Register(new Scheduler());
		Loader::Register(new GSC());
		Loader::Register(new Movement());
		Loader::Register(new Weapons());
		Loader::Register(new Discord());
		Loader::Register(new UIScript());
		Loader::Register(new ServerCommand());
		Loader::Register(new LocalizedStrings());
		Loader::Register(new Toast());
		Loader::Register(new Achievements());
		Loader::Register(new Events());
		Loader::Register(new Updater());

		Loader::Pregame = false;
		// Make sure preDestroy is called when the game shuts down
		Scheduler::OnGameShutdown(Loader::PreDestroy);
	}

	void Loader::Uninitialize()
	{
		Loader::Uninitializing = true;
		Loader::PreDestroyNoPostGame();

		std::reverse(Loader::Components.begin(), Loader::Components.end());
		for (auto component : Loader::Components)
		{
			delete component;
		}

		Loader::Components.clear();
		Utils::Memory::GetAllocator()->clear();
		Loader::Uninitializing = false;
	}

	void Loader::PreDestroy()
	{
		if (!Loader::Postgame)
		{
			Loader::Postgame = true;

			Steam::SteamAPI_Shutdown();

			auto components = Loader::Components;

			std::reverse(components.begin(), components.end());
			for (auto component : components)
			{
				component->preDestroy();
			}
		}
	}

	void Loader::PreDestroyNoPostGame()
	{
		if (!Loader::Postgame)
		{
			auto components = Loader::Components;

			std::reverse(components.begin(), components.end());
			for (auto component : components)
			{
				component->preDestroy();
			}

			Loader::Postgame = true;
		}
	}

	void Loader::Register(Component* component)
	{
		if (component)
		{
			Loader::Components.push_back(component);
		}
	}

	bool Loader::BinaryCheck()
	{
		std::string data;
		if (!Utils::IO::ReadFile("iw3sp.exe", &data))
		{
			MessageBoxA(nullptr, "Failed to read game binary - iw3sp.exe", nullptr, MB_ICONERROR);
			Utils::Library::Terminate();
			return false;
		}

		const auto hash = Utils::Cryptography::SHA1::Compute(data, true);
		if ((hash != "5F06ACF8FA84D20E902BBBF423466C6752D920BF"))
		{
			MessageBoxA(nullptr, "Your iw3sp.exe is incompatible with this client.", nullptr, MB_ICONERROR);
			Utils::Library::Terminate();
			return false;
		}

		return true;
	}
}