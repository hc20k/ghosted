// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "ghosts.h"
#include <cstdio>
#include <string>

vec3 savedPosition;
bool weaponGlitch = false;

typedef void(*fCbuf_AddText)(int client, char* cmd);
typedef void(*fSV_GameSendServerCommand)(int client, char* cmd);

void Cbuf_AddText(int client, const char* cmd) {
    fCbuf_AddText addTextFunc = (fCbuf_AddText)(pCbuf_AddText);
    addTextFunc(0, _strdup(cmd));
}

void SV_GameSendServerCommand(int client, const char* cmd) {
    //TODO: FIX OFFSET
    fSV_GameSendServerCommand sendCommand = (fSV_GameSendServerCommand)(pSV_GameSendServerCommand);
    sendCommand(client, _strdup(cmd));
}

int main(PVOID module) {
    FILE* fDummy;

    AllocConsole();
    SetConsoleTitle(L"ghosted v1");
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);

    printf("ghosted v1 by h\nhave fun!\n");

    player localPlayer = *(player*)LP_BASE;

    *(int*)(SQUAD_POINTS) = 777;
    Cbuf_AddText(0, "cl_yawspeed 950\n"); // for trickshotting

    //set motd
    motd gMOTD = *(motd*)MAIN_MOTD;
    strcpy_s(gMOTD.title, "^1ghosted v1 by h!");
    strcpy_s(gMOTD.description, "^1have fun playing this dead game <3");
    *(motd*)MAIN_MOTD = gMOTD;

    Cbuf_AddText(0, "say \"^5ghosted v1 Loaded\";\n");

    // monitor binds TODO: actual button monitoring
    while (!GetAsyncKeyState(VK_INSERT)) {

        if (*(BYTE*)(DPAD_BUTTON_STATES) == 1) {
            // DPAD UP
            *(vec3*)(LP_ORIGIN) = savedPosition;
            Cbuf_AddText(0, "say \"^3Loaded location!\";\n");
            Sleep(300);
        }

        if (*(BYTE*)(DPAD_BUTTON_STATES) == 2) {
            // DPAD DOWN
            savedPosition = *(vec3*)(LP_ORIGIN);
            Cbuf_AddText(0, "say \"^2Saved location!\";\n");
            Sleep(300);
        }

        if (*(BYTE*)(DPAD_BUTTON_STATES) == 4) {
            // DPAD LEFT
            // FLY UP
            (*(vec3*)(LP_ORIGIN)).z ++;
        }

        if (GetAsyncKeyState(VK_F9)) {
            weaponGlitch = !weaponGlitch;
            if (weaponGlitch == true)
                Cbuf_AddText(0, "say \"Weapon Glitch [^2ON^7]\";\n");
            else
                Cbuf_AddText(0, "say \"Weapon Glitch [^1OFF^7]\";\n");
            Sleep(100);
        }

        // MODS
        if (weaponGlitch == true) {
            weap_state weaponState = *(weap_state*)(LP_WEAP_STATE);

            if (weaponState != SWITCHING && weaponState != RELOADING) {
                *(weap_state*)(LP_WEAP_STATE) = IDLE;
            }
        }

        // Unlimited Stock Ammo for trickshotting
        *(int*)(WAMMO_STOCK(0)) = 777;
        *(int*)(WAMMO_STOCK(1)) = 777;

        // God mode
        *(int*)(LP_HEALTH) = 0; // 0 for no target & no sui | 9999 for regular

        Sleep(1);
    }

    printf("Unloaded. Goodbye :)\n");
    Cbuf_AddText(0, "say \"^1Cheat unloaded!\";\n");

    FreeConsole();
    FreeLibraryAndExitThread(static_cast<HMODULE>(module), 1);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

