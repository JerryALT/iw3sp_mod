#include "STDInc.hpp"

namespace Main
{
    static Utils::Hook EntryPointHook_;

    void Initialize()
    {
        Main::EntryPointHook_.uninstall();
        Components::Loader::Initialize();
    }

    void Uninitialize()
    {
        Components::Loader::Uninitialize();
    }
}

__declspec(naked) void start_entry_point()
{
    __asm
    {
        call	Main::Initialize;

        mov		eax, 0x643AFB;
        jmp		eax;
    }
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
    auto* _module = reinterpret_cast<char*>(0x400000);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        if (Components::Loader::BinaryCheck())
        {
            Utils::SetEnvironment();
            Steam::Proxy::RunMod();

            DWORD oldProtect;
            VirtualProtect(_module + 0x1000, 0x0FB7000, PAGE_EXECUTE_READWRITE, &oldProtect);

            Main::EntryPointHook_.initialize(0x643AFB, start_entry_point)->install();
            Utils::Hook(0x643AFB, start_entry_point, HOOK_JUMP).install()->quick();
        }
        else
            return FALSE;
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        Main::Uninitialize();
    }

    return TRUE;
}