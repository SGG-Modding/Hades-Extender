#include <Windows.h>
#include "./Core/core.h"

/* Local and global variable definitions */
HMODULE dll;
HWND g_hwnd;
Proxy proxy;

/* Our main module initializtion */
VOID Initialize( VOID )
{
	/* Initialize our debug window if enabled */
	DEBUG_INIT("Hades - Debugger");

	/* Assign our global window handle to the main Hades window */
	g_hwnd = (HWND)FindWindowA("The Forge", NULL);

	/* Initialize our core */
	if (!Core::Initialize())
	{
		MessageBoxA(NULL, "Failed to initialize core!", "Hades Extender", NULL);
	}
}

/* Our primary entrypoint called by SDL.dll */
BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
	/* Return if reason is not attach */
	if (_Reason != DLL_PROCESS_ATTACH) return FALSE;
	/* Assign dll to variable to future use */
	dll = (HMODULE)_DllHandle;
	/* Prevent this module from getting notified upon dll load/unload */
	DisableThreadLibraryCalls(dll);

	/* Note: We might need to make an actual thread in the future! */
	/* Initialize our module */
	Initialize();

	/* Return success message to dispatcher */
	return TRUE;
}