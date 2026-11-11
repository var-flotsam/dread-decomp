#pragma once

// CEvent: functions of this translation unit that other files call (tools/tu_map.py unit 41,
// 0x7100066630-0x71000684e4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkWideStr_0x20;
void fn_7100067ea4(void* self);  // The event base's constructor.
void fn_7100066e44(void* text);
void fn_7100066b38(UnkWideStr_0x20* self, const char* str);
void fn_710006663c(u64* out, const char* str);
void fn_7100066828(void* thread, u32 a);
void fn_7100067ed4(void* self);  // Derived classes call it as their base's destructor.
void fn_710006686c(void* thread);
void fn_7100066cac(void* self, const void* other);
u32 fn_7100066930(u32 n);
bool fn_71000669a0(u32 n);
