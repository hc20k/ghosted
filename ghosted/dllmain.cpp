// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "ghosts.h"
#include "dvars.h"
#include <cstdio>
#include <string>
#include <iostream>

vec3 savedPosition;
bool weaponGlitch = false;

typedef void(*fCbuf_AddText)(int client, char* cmd);
typedef void(*fSV_GameSendServerCommand)(int client, int type, char* cmd);

void Cbuf_AddText(int client, const char* cmd) {
    ((void(__cdecl*)(int, char*))pCbuf_AddText)(client, _strdup(cmd));
}

void SV_GameSendServerCommand(int client, const char* cmd) {
    ((void(__cdecl*)(int, int, char*))pSV_GameSendServerCommand)(client, 1, _strdup(cmd));
}

bool Key_IsDown(int button) {
    return ((bool(__cdecl*)(int, int))pKey_IsDown)(0, button);
}

void debugConsole() {
    FILE* fDummy;
    AllocConsole();
    SetConsoleTitle(L"ghosted v1");
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
}

void openIW6Console() {
    ((void(__cdecl*)(HINSTANCE))0x140502C20)(GetModuleHandle(NULL)); //CreateConsole
    ((void(__cdecl*)(void))0x140503130)(); //ShowConsole
}

void PatchFunctions() {
    // Patches SV_BotThink so bots don't kill anymore
    char nop5[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
    memcpy((void*)0x140463E96, (void*)nop5, sizeof(nop5));


}

int lastWeapState = 0;
void MW2mechanics(PlayerActiveWeaponState* weapState) {

    if (weapState->weaponState == 0x1 || weapState->weaponState == 0x2 || weapState->weaponState == 0x5) {
        weapState->weaponState = 0x0;
        weapState->weaponTime = 0;
    }

    if (lastWeapState != weapState->weaponState) {
        if (lastWeapState != 0 && lastWeapState != 25  && lastWeapState != 24 && weapState->weaponState == 3) {
            weapState->weaponTime = 0;
        }
    }

    lastWeapState = weapState->weaponState;

    //printf("state: %d | time: %d | flags: %d\n", weapState->weaponState, weapState->weaponTime, weapState->weapHandFlags);
}

int main(PVOID module) {
    debugConsole();

    PatchFunctions();

    SV_GameSendServerCommand(0, "c \"^6ghosted v1 Loaded!\"");

    level_locals_t locals = *(level_locals_t*)(pLevelLocals);
    animScriptData_t* scriptData = (animScriptData_t*)pGlobalScriptData;
    gclient_s* me = ((gclient_s * (__cdecl*)(int))pG_GetPlayerState)(0);
    playerState_s* ps = (playerState_s*)me->ps;
    gentity_s* ent = (gentity_s*)(pG_Entities);
    
    //BG_GetWeaponAnimArraysNonConst

    while (!GetAsyncKeyState(VK_INSERT)) {

        if (Key_IsDown(B_BUTTON) && Key_IsDown(DOWN_DPAD)) {
            // Save Location
            savedPosition = *(vec3*)(LP_ORIGIN);
            SV_GameSendServerCommand(0, "e \"^6Saved Location\"");
            Sleep(500);
        }

        if (Key_IsDown(B_BUTTON) && Key_IsDown(UP_DPAD)) {
            // Load Location
            if (savedPosition.x != NULL && savedPosition.y != NULL && savedPosition.z != NULL) {
                *(vec3*)(LP_ORIGIN) = savedPosition;
            }
            else {
                SV_GameSendServerCommand(0, "e \"^1You haven't saved a location yet!\"");
            }
            Sleep(500);
        }

        if (Key_IsDown(L_TRIGGER) && Key_IsDown(R_STICK)) {
            // TP All Bots to Origin
            vec3 currentOrigin = *(vec3*)(&*ent->r.currentOrigin);

            for (int i = 0; i < 18; i++) {
                // sizeof(gentity_s) = 0x2F0
                gentity_s* iEnt = (gentity_s*)(pG_Entities + (0x2F0 * i));
                if (iEnt != NULL && iEnt != ent) {
                    *(vec3*)(&*iEnt->r.currentOrigin) = currentOrigin; // Doesn't work for some reason
                }
            }
            SV_GameSendServerCommand(0, "e \"^2Teleported all bots to you.\"");

            Sleep(500);
        }

        if (Key_IsDown(VK_F9)) {
            Cbuf_AddText(-1,"fast_restart;\n");
            Sleep(500);
        }

        if (me != NULL && ent != NULL) {
            // GOD MODE
            ent->takedamage &= ~0x2;
            ent->takedamage &= ~0x1;

            //MW2mechanics(ps->weapState);

            *(int*)(WAMMO_STOCK(0)) = 777;
            *(int*)(WAMMO_STOCK(1)) = 777;
            *(int*)(LETHAL_COUNT) = 777;
        }

        Sleep(50);
    }

    printf("Unloaded. Goodbye :)\n");
    SV_GameSendServerCommand(0, "c \"^1Cheat unloaded!\"");

    FreeConsole();
    FreeLibraryAndExitThread(static_cast<HMODULE>(module), 1);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    DisableThreadLibraryCalls(hModule);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {

            
    }
    case DLL_THREAD_ATTACH: {

        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
        break;
    }
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

