#pragma once

// CCollider: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2503,
// 0x7100d361bc-0x7100d454f8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMatrix44;
struct UnkShape;
struct UnkObj_71007e44b8;
struct UnkTriggerCollider_b0058;
void fn_7100d36d30(UnkTriggerCollider_b0058* self, void* shape);
void fn_7100d36624(UnkObj_71007e44b8* self);  // Constructor.
extern char g_unk_7101aa85b0[];
extern char g_unk_7101aa8660[];
UnkMatrix44 fn_7100d36a2c(UnkShape* shape);
