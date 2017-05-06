// Mod loader for Freeware Cave Story
// Copyright © 2017 Clownacy

#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "log.h"
#include "mod_list.h"
#include "redirect_org_files.h"
#include "settings.h"

HMODULE this_hmodule;

void LoadMod(const char* const filename)
{
		PrintDebug("Loading mod '%s'...\n", filename);

		char mod_folder_relative[5 + strlen(filename) + 1 + 1];
		sprintf(mod_folder_relative, "mods\\%s\\", filename);

		size_t mod_folder_length = GetFullPathName(mod_folder_relative, 0, NULL, NULL);
		char *mod_folder = malloc(mod_folder_length);
		GetFullPathName(mod_folder_relative, mod_folder_length, mod_folder, NULL);

		PrintDebug("  mod_folder: '%s'\n", mod_folder);

		AddToModList(mod_folder);

		char mod_path[strlen(mod_folder) + strlen(filename) + 1];
		sprintf(mod_path, "%s%s", mod_folder, filename);

		PrintDebug("  mod_path: '%s'\n", mod_path);

		// Load mod DLL
		HMODULE hmodule = LoadLibrary(mod_path);
		if (hmodule == NULL)
		{
			free(mod_folder);
			LPVOID error_message;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&error_message, 0, NULL);
			PrintError("Could not load mod '%s'\n  Windows reports: %s\n", filename, error_message);
			LocalFree(error_message);
			return;
		}

		// Get DLL entry point
		void (*ModEntry)(HMODULE, void*, const char* const) = (void (*)(HMODULE, void*, const char* const))GetProcAddress(hmodule, "ModEntry");
		if (ModEntry == NULL)
		{
			free(mod_folder);
			PrintError("Mod '%s' did not contain a valid entry point (\"ModEntry\")\n", filename);
			return;
		}

		// Run mod
		ModEntry(this_hmodule, ReadSettings(filename), mod_folder);
}

__declspec(dllexport) void init(void)
{
	InitLogging();
	//RedirectOrgFiles();

	SetDllDirectory("mods/_deps");

	FILE *mod_list = fopen("mods/mods.txt", "r");

	if (mod_list == NULL)
	{
		PrintError("Could not find mods.txt\n");
		return;
	}

	// Scan through mods.txt, loading each DLL listed
	char filename[MAX_PATH];
	while (fgets(filename, MAX_PATH, mod_list) != NULL)
	{
		// Get path of mod DLL
		filename[strcspn(filename, "\r\n")] = '\0';	// Trim newline characters
		LoadMod(filename);
	}

	fclose(mod_list);
	SetDllDirectory(NULL);
}

BOOL APIENTRY DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		this_hmodule = hinstDll;

	return TRUE;
}
