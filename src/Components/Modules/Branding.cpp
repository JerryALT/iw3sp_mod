#include "STDInc.hpp"

namespace Components
{
	void version_of_the_game()
	{
		const char* text = "IW3SP_MOD " VERSION;

		const float color[4] = { 0.0f, 0.80f, 0.0f, 0.69f };
		const float shadowColor[4] = { 0.0f, 0.0f, 0.0f, 0.69f };

		const float fontScale = 0.2f;

		auto* const placement = Game::ScrPlace_GetUnsafeFullPlacement();
		const char* fontname = Game::UI_GetFontHandle(placement, fontScale);
		Game::Font_s* fontHandle = Game::R_RegisterFont("fonts/objectiveFont", sizeof("fonts/objectiveFont"));

		const auto width = Game::UI_TextWidth(text, 0, fontHandle, fontScale);
		const auto height = Game::UI_TextHeight(fontHandle, fontScale);

		float offs_x = 2.5f;
		float offs_y = 13.5f;

		Game::UI_DrawText(placement, text, 0x7FFFFFFF, fontHandle, offs_x + 1.0f, offs_y + 1.0f, 0, 0, fontScale, shadowColor, 0);
		Game::UI_DrawText(placement, text, 0x7FFFFFFF, fontHandle, offs_x, offs_y, 0, 0, fontScale, color, 0);
	}

	void version_of_the_game_stub2()
	{
		version_of_the_game();
		Utils::Hook::Call<void()>(0x438B20)();
	}

	Branding::Branding()
	{
		//Branding stuffs
		//	Game window name
		Utils::Hook::Set<const char*>(0x5D9680, "IW3SP_MOD");
		//	Game console name (external console)
		Utils::Hook::Set<const char*>(0x5963F7, "IW3SP Modification");

		//	Game console name (~)
		Utils::Hook::Set<const char*>(0x43B9E1, "IW3SP_MOD: > ");
		//	Short version
		Utils::Hook::Set<const char*>(0x534FE3, "1.5");

		Utils::Hook::Set<const char*>(0x53535E, "// generated by IW3SP_MOD\n");
		Utils::Hook::Set<const char*>(0x5353F0, "// generated by IW3SP_MOD\n");


		//	SCR_UpdateFrame hooking
		Utils::Hook(0x447888, version_of_the_game_stub2, HOOK_CALL).install()->quick();
	}

	Branding::~Branding()
	{
	}
}