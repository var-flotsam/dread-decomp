#pragma once

// UnkTU1501: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1501,
// 0x71009dc3bc-0x71009f16a8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMove_b152;
struct UnkSamusSettings;
struct UnkFlagged;
struct UnkHelper;
struct UnkSamusMovement;
struct UnkGroundMovement_ovb0138;
struct UnkSamusMovementUpdate;
bool fn_71009e6434(UnkHelper* helper);
bool fn_71009e75e8(UnkFlagged*);
bool fn_71009e6f9c(UnkHelper* helper);
s32 fn_71009e8590(UnkFlagged* object);
void fn_71009e0160(UnkSamusMovement* self, bool flag, f32 dt);
bool fn_71009e1390(void* obj);
void fn_71009e4598(UnkSamusMovementUpdate* self);
bool fn_71009e84e4(char* self);
void fn_71009e85f4(void* self, bool flag);
bool fn_71009eaa88(void* movement);
extern UnkSamusSettings g_unk_7172740de0;
// Clears the movement's air state on landing, stops the gun and lets the spin jump reset the animation.
void fn_71009ea070(UnkGroundMovement_ovb0138* movement);
f32 fn_71009e754c(UnkMove_b152* self);
bool fn_71009e12d0(void*);
