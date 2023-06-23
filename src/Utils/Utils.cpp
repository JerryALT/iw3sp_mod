#include "STDInc.hpp"

namespace Utils
{
	void SetLegacyEnvironment()
	{
		wchar_t binaryPath[512]{};
		GetModuleFileNameW(GetModuleHandleW(nullptr), binaryPath, sizeof(binaryPath) / sizeof(wchar_t));

		auto* exeBaseName = std::wcsrchr(binaryPath, L'\\');
		exeBaseName[0] = L'\0';

		SetCurrentDirectoryW(binaryPath);
	}

	void SetEnvironment()
	{
		char* buffer{};
		std::size_t size{};
		if (_dupenv_s(&buffer, &size, "COD4_INSTALL") != 0 || buffer == nullptr)
		{
			SetLegacyEnvironment();
			return;
		}

		const auto _0 = gsl::finally([&] { std::free(buffer); });

		SetCurrentDirectoryA(buffer);
		SetDllDirectoryA(buffer);
	}

	void SafeShellExecute(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd)
	{
		[=]()
		{
			__try
			{
				ShellExecuteA(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
			}
			__finally
			{
			}
		}();

		std::this_thread::yield();
	}

	void OpenUrl(const std::string& url)
	{
		SafeShellExecute(nullptr, "open", url.data(), nullptr, nullptr, SW_SHOWNORMAL);
	}
}