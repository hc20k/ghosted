#pragma once
/* dvar->flags */
#define DVAR_ARCHIVE		(1 << 0)	// 0x0001
#define DVAR_USERINFO		(1 << 1)	// 0x0002
#define DVAR_SERVERINFO		(1 << 2)	// 0x0004
#define DVAR_SYSTEMINFO		(1 << 3)	// 0x0008
#define DVAR_INIT		(1 << 4)	// 0x0010
#define DVAR_LATCH		(1 << 5)	// 0x0020
#define DVAR_ROM		(1 << 6)	// 0x0040
#define DVAR_CHEAT		(1 << 7)	// 0x0080
#define DVAR_DEVELOPER		(1 << 8)	// 0x0100
#define DVAR_SAVED		(1 << 9)	// 0x0200
#define DVAR_NORESTART		(1 << 10)	// 0x0400
#define DVAR_CHANGEABLE_RESET	(1 << 12)	// 0x1000
#define DVAR_EXTERNAL		(1 << 14)	// 0x4000
#define DVAR_AUTOEXEC		(1 << 15)	// 0x8000

struct vec4_t {
    float x, y, z, z2;
};

enum dvarType_t
{
    DVAR_TYPE_INVALID = 0x0,
    DVAR_TYPE_BOOL = 0x1,
    DVAR_TYPE_FLOAT = 0x2,
    DVAR_TYPE_FLOAT_2 = 0x3,
    DVAR_TYPE_FLOAT_3 = 0x4,
    DVAR_TYPE_FLOAT_4 = 0x5,
    DVAR_TYPE_INT = 0x6,
    DVAR_TYPE_ENUM = 0x7,
    DVAR_TYPE_STRING = 0x8,
    DVAR_TYPE_COLOR = 0x9,
    DVAR_TYPE_INT64 = 0xA,
    DVAR_TYPE_LINEAR_COLOR_RGB = 0xB,
    DVAR_TYPE_COLOR_XYZ = 0xC,
    DVAR_TYPE_COUNT = 0xD,
};

union DvarLimits
{
    struct {
        int stringCount;
        const char** strings;
    } enumeration;

    struct {
        int min;
        int max;
    } integer;

    struct {
        float min;
        float max;
    } value, vector;

    struct {
        __int64 min;
        __int64 max;
    } integer64;
};

union DvarValue
{
    bool enabled;
    int integer;
    unsigned int unsignedInt;
    __int64 integer64;
    unsigned __int64 unsignedInt64;
    float value;
    vec4_t vector;
    const char* string;
    char color[4];
};

#pragma pack(push, 4)
struct dvar_t
{
    const char* name;
    const char* description;
    int hash;
    unsigned int flags;
    dvarType_t type;
    bool modified;
    DvarValue current;
    DvarValue latched;
    DvarValue reset;
    DvarLimits domain;
    dvar_t* hashNext;
    int unknown3;
};