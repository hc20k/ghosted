#pragma once
#include <cstdint>

// PLAYER
#define LP_BASE 0x14444CF00
#define LP_ORIGIN LP_BASE+0x2C 
#define LP_WEAP_STATE LP_BASE+0x2CC // haha addy is D1CC
#define LP_HEALTH 0x14427A2BC
#define SQUAD_POINTS 0x1445A34A0

#define FFA_SCORE(entity) 0x144B34720

#define WAMMO_STOCK(index) 0x14444D378+(index*0x10)
#define WAMMO_CLIP(index) WAMMO_STOCK(index)+0x110+(index*0x10)

// FUNC
#define pCbuf_AddText 0x1403F6B50
#define pSV_GameSendServerCommand 0x14459F5B8 //I NEED TO REFIND THIS OFFSET SHIT CRASHES

// MISC
#define MAIN_MOTD 0x14457B870

#define DPAD_BUTTON_STATES 0x006E1E50

enum weap_state { IDLE, RELOADING, UNKNOWN, SWITCHING, };

struct motd {
	char title[64];
	char description[512];
};

struct vec3 {
	float x, y, z;
};

struct player {
	char _padding[0x2C];
	vec3 origin;
};
