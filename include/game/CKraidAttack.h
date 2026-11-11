#pragma once

// CKraidAttack: functions of this translation unit that other files call (tools/tu_map.py unit 912,
// 0x71005f1d00-0x71005f2360). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkKraidAttack;
struct UnkSelf_kt41;
void fn_71005f2018(char* self);  // Derived classes call it as their base's restart (a thunk to fn_71003608ac).
bool fn_71005f20e4(UnkSelf_kt41*);
bool fn_71005f2014(UnkSelf_kt41* self, void* a, void* b);
void fn_71005f201c(UnkKraidAttack* self, f32 dt);
bool fn_71005f2024(UnkSelf_kt41* self);
bool fn_71005f2084(UnkSelf_kt41* self);
