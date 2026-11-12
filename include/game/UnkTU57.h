#pragma once

// UnkTU57: functions of this translation unit that other files call (tools/tu_map.py unit 57,
// 0x710007e6c4-0x710007fdb4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct UnkPooledStrId;
struct UnkCompared_0x30;
struct UnkString_7100fc4b34;
bool fn_710007f120(const UnkCompared_0x30* a, const UnkCompared_0x30* b);
void fn_710007f634(void* self, u32 value);  // Appends a number.
void fn_710007eba0(UnkString_7100fc4b34* self, u32 size);
void fn_710007eb6c(void* self, const void* id);
void fn_710007f5e0(void* self, f32 value);  // Appends a number.
void fn_710007e6c4(void* self);  // Constructs an empty base::global::CRntString.
void fn_710007f564(void* self, const char* str);
base::global::CRntString fn_710007ef90(const base::global::CRntString* a, const char* b);
base::global::CRntString fn_710007f050(const char* a, const base::global::CRntString* b);
s32 fn_710007facc(const base::global::CRntString* self, char c, u32 start);
s32 fn_710007fb10(const base::global::CRntString* self, char c, s32 start);
void fn_710007eb20(base::global::CRntString* self, const UnkPooledStrId* id);
s32 fn_710007fb54(const base::global::CRntString* self, const char* str, u32 start);
bool fn_710007fc80(const base::global::CRntString* self, const base::global::CRntString* prefix);
