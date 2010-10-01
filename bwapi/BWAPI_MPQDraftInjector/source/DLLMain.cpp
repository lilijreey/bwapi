#include "MPQDraftPlugin.h"
#include <windows.h>
#include <string>
#include <assert.h>
#include <stdio.h>

#include "../../svnrev.h"
#include "../../starcraftver.h"

#define THEPLUGINID 0x10001000

const char* plugin_name = "BWAPI Injector (" STARCRAFT_VER ") r" SVN_REV_STR;

void BWAPIError(const char *format, ...)
{
  char buffer[MAX_PATH];
  va_list ap;
  va_start(ap, format);
  vsnprintf_s(buffer, MAX_PATH, MAX_PATH, format, ap);
  va_end(ap);

  FILE* f = fopen("bwapi-error.txt", "a+");
  fprintf(f, "%s\n", buffer);
  fclose(f);
  MessageBox(NULL, buffer, "Error", MB_OK | MB_ICONERROR);
}

class MPQDraftPluginInterface : public IMPQDraftPlugin
{
    HINSTANCE hInstance;
  public:
    BOOL WINAPI Identify(LPDWORD pluginID)
    {
      if (!pluginID)
        return FALSE;

      *pluginID = THEPLUGINID;
      return TRUE;
    }
    BOOL WINAPI GetPluginName(LPSTR pPluginName, DWORD namebufferlength)
    {
      if ( !pPluginName || namebufferlength < strlen(plugin_name) )
        return FALSE;

      strcpy(pPluginName, plugin_name);
      return TRUE;
    }
    BOOL WINAPI CanPatchExecutable(LPCSTR exefilename)
    {
      //Generally you check if the version is right here
      return TRUE;
    }
    BOOL WINAPI Configure(HWND parentwindow)
    {
      //Goes here when they hit Configure
      if ( !ShellExecute(NULL, "open", "..\\bwapi-data\\bwapi.ini", NULL, NULL, SW_SHOWNORMAL) )
        return FALSE;
      return TRUE;
    }
    BOOL WINAPI ReadyForPatch()
    {
      //Right before it opens up SC, checks for panic?
      return TRUE;
    }
    BOOL WINAPI GetModules(MPQDRAFTPLUGINMODULE* pluginmodules, LPDWORD nummodules)
    {
      //Weird shit, i doubt you'll use it
      if (!nummodules)
        return FALSE;
      *nummodules = 0;
      return TRUE;
    }
    BOOL WINAPI InitializePlugin(IMPQDraftServer* server)
    {
      char envBuffer[MAX_PATH];
      if ( !GetEnvironmentVariable("ChaosDir", envBuffer, MAX_PATH) )
        if ( !GetCurrentDirectoryA(MAX_PATH, envBuffer) )
          BWAPIError("Could not find ChaosDir or current directory.");

      strcat(envBuffer, "\\BWAPI.dll");
      if ( !LoadLibrary(envBuffer) )
        BWAPIError("Could not find and/or load BWAPI.dll.");
      return TRUE;
    }
    BOOL WINAPI TerminatePlugin()
    {
      //Called when starcraft closes
      return TRUE;
    }
    void WINAPI SetInstance(HINSTANCE hInst)
    {
      hInstance = hInst;
    }
};

MPQDraftPluginInterface thePluginInterface;

__declspec(dllexport) BOOL APIENTRY DllMain( HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
    thePluginInterface.SetInstance(hInstance);
  return TRUE;
}

BOOL WINAPI GetMPQDraftPlugin(IMPQDraftPlugin** lppMPQDraftPlugin)
{
  *lppMPQDraftPlugin = &thePluginInterface;
  return TRUE;
}
