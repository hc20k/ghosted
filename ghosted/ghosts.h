#pragma once
#include <cstdint>
#include "Reversed/playerState_s.h"
#include "Reversed/level_locals_t.h"

// PLAYER
#define LP_BASE 0x14444CF00
#define LP_ORIGIN LP_BASE+0x2C 
#define LP_ANGLES LP_BASE+0x198
#define LP_HEALTH 0x14427A2BC
#define SQUAD_POINTS 0x1445A34A0

#define WAMMO_STOCK(index) 0x14444D378+(index*0x10)
#define WAMMO_CLIP(index) WAMMO_STOCK(index)+0x110+(index*0x10)
#define LETHAL_COUNT 0x14444D448
#define TACTICAL_COUNT 0x14444D458

// DVARS
#define FFA_SCORELIMIT 0x14983E2E4
#define FFA_TIMELIMIT 0x14983E2DE
#define G_GAMETYPE 0x1478114C0

// FUNC
#define pCbuf_AddText 0x1403F6B50
#define pSV_GameSendServerCommand 0x1404758C0
#define pSys_CreateConsole 0x140502C20
#define pDvar_SetStringByName 0x1404ED140
#define pKey_IsDown 0x1402C4090
#define pLevelLocals 0x1443F4600
#define pG_GetPlayerState 0x14039EFC0 //returns gclient_s ptr

// MISC
#define MAIN_MOTD 0x14457B870
#define BUILD_INFO 0x1446BEEA4
#define ADD_TO_CONSOLE_BUFF 0x1405028C0
#define DPAD_BUTTON_STATES 0x006E1E50
#define pG_Entities 0x14427A0E0
#define pGlobalScriptData 0x141722D50 //animScriptData_t

struct animation_s {
	char* name;
	__int64 movetype;
	float moveSpeed;
	int nameHash;
	int flags;
	__int16 initialLerp;
	unsigned __int16 duration;
	unsigned __int16 localMeleeVictimAnimIndex;
	char noteType;
	char aimSet;
	char leanSet;
	char turns;
	char twitches;
	char syncGroup;
};

struct animScriptData_t {
	animation_s* animations;
	char dontwannawritetherest[0x44210];
};

enum GAMEPAD_BTNS {
	A_BUTTON = 1,
	B_BUTTON = 2,
	X_BUTTON = 3,
	Y_BUTTON = 4,
	LEFT_BUMPER = 5,
	RIGHT_BUMPER = 6,
	UP_DPAD = 20,
	DOWN_DPAD = 21,
	LEFT_DPAD = 22,
	RIGHT_DPAD = 24,
	L_STICK = 16,
	R_STICK = 17,
	L_TRIGGER = 18,
	R_TRIGGER = 19
};

enum weap_state { IDLE, RELOADING, UNKNOWN, SWITCHING, };

struct motd {
	char title[64];
	char description[512];
};

struct vec3 {
	float x, y, z;
};