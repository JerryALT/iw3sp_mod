#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define _USE_MATH_DEFINES
#define NOMINMAX

#define CLIENT_CONFIG "iw3sp_mod_config.cfg"
#define MOD_CONFIG "mods_config.cfg"

// Version number
#include <version.hpp>

#include <windows.h>
#include <WinSock2.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <winsock.h>
#include <stdint.h>
#include <ShlObj.h>
#include <timeapi.h>
#include <functional>
#include <mutex>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <ranges>
#include <list>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdio>
#include <utility>

#include <gsl/gsl>
#include <curl/curl.h>

#pragma warning(push)
#pragma warning(disable: 4091)
#pragma warning(disable: 4244)
#include <sstream>
#include <map>
#include <thread>
#include <filesystem>
#include <optional>
#include <queue>
#pragma warning(pop)

using namespace std::literals;

#define __thread __declspec(thread)

#include <Urlmon.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <shellapi.h>
#include <wininet.h>
#include <Shlwapi.h>
#include <d3d9.h>
#include <d3dx9tex.h>
#include <dwmapi.h>

#include <tomcrypt.h>
#include <json.hpp>

#pragma comment (lib, "dwmapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "Winmm.Lib")


// Ignore the warnings
#pragma warning(push)
#pragma warning(disable: 4005)
#pragma warning(disable: 4091)
#pragma warning(disable: 4100)
#pragma warning(disable: 4244)
#pragma warning(disable: 4389)
#pragma warning(disable: 4702)
#pragma warning(disable: 4800)
#pragma warning(disable: 4996) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 5054)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#pragma warning(disable: 6031)
#pragma warning(disable: 6255)
#pragma warning(disable: 6258)
#pragma warning(disable: 6386)
#pragma warning(disable: 6387)
#pragma warning(disable: 26812)


//Game Structure
#include "Utils\IO.hpp"
#include "Utils\Time.hpp"
#include "Utils\Memory.hpp"
#include "Utils\Cryptography.hpp"
#include "Utils\Hooking.hpp"
#include "Utils\Function.hpp"
#include "Utils\String.hpp"
#include "Utils\Thread.hpp"
#include "Utils\Utils.hpp"
#include "Utils\Library.hpp"
#include "Utils\HTTP.hpp"

#include "Steam\Steam.hpp"

#include "Game\Structs.hpp"
#include "Game\Functions.hpp"
#include "Components\Modules\Dvars.hpp"

#include "Components\Loader.hpp"