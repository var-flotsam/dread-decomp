// The CBasicLifeComponent unit. fn_71008a40c0 turns a damage source's name into its flag (one bit each, in
// order from POWER_BEAM), "ALL" into every bit and anything else into none. Names are placeholders.
#include <string.h>

#include "types.h"

u32 fn_71008a40c0(const char* text) {
    if (strcmp(text, "POWER_BEAM") == 0)
        return 0x1;
    if (strcmp(text, "ICE_MISSILE") == 0)
        return 0x2;
    if (strcmp(text, "MISSILE") == 0)
        return 0x4;
    if (strcmp(text, "SUPER_MISSILE") == 0)
        return 0x8;
    if (strcmp(text, "BOMB") == 0)
        return 0x10;
    if (strcmp(text, "POWER_BOMB") == 0)
        return 0x20;
    if (strcmp(text, "SCREW_ATTACK") == 0)
        return 0x40;
    if (strcmp(text, "SPECIAL_ENERGY") == 0)
        return 0x80;
    if (strcmp(text, "MELEE_CHARGE_BEAM") == 0)
        return 0x100;
    if (strcmp(text, "MULTI_LOCKON_MISSILE") == 0)
        return 0x200;
    if (strcmp(text, "CHARGE_BEAM") == 0)
        return 0x400;
    if (strcmp(text, "MELEE") == 0)
        return 0x800;
    if (strcmp(text, "WAVE_BEAM") == 0)
        return 0x1000;
    if (strcmp(text, "WIDE_BEAM") == 0)
        return 0x2000;
    if (strcmp(text, "PLASMA_BEAM") == 0)
        return 0x4000;
    if (strcmp(text, "SHINE_SPARK") == 0)
        return 0x8000;
    if (strcmp(text, "HYPER_BEAM") == 0)
        return 0x10000;
    if (strcmp(text, "HEAT") == 0)
        return 0x20000;
    if (strcmp(text, "DASH_MELEE") == 0)
        return 0x40000;
    if (strcmp(text, "EMMY") == 0)
        return 0x80000;
    if (strcmp(text, "LAVA") == 0)
        return 0x100000;
    if (strcmp(text, "GRAPPLE_BEAM") == 0)
        return 0x200000;
    if (strcmp(text, "LASER") == 0)
        return 0x400000;
    if (strcmp(text, "SPIN_ATTACK") == 0)
        return 0x800000;
    if (strcmp(text, "FREEZE") == 0)
        return 0x1000000;
    if (strcmp(text, "SPEED_BOOSTER") == 0)
        return 0x2000000;
    if (strcmp(text, "ANTI_SCREW_ATTACK") == 0)
        return 0x4000000;
    if (strcmp(text, "DIFFUSION_BEAM") == 0)
        return 0x8000000;
    if (strcmp(text, "CONTINUOUS") == 0)
        return 0x10000000;
    if (strcmp(text, "LINE_BOMB") == 0)
        return 0x20000000;
    return strcmp(text, "ALL") == 0 ? 0xffffffff : 0;
}
