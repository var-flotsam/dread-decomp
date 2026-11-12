#pragma once

// UnkTU54: functions of this translation unit that other files call (tools/tu_map.py unit 54,
// 0x71000769f0-0x710007b430). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkKeyedEntry;
struct UnkMatrix44;
struct UnkXform;
struct UnkAim;
struct UnkMat4f;
f32 fn_710007a1d0(f32 angle);  // Wraps an angle into (-pi, pi].
void fn_7100077bbc(UnkMat4f* m);
void fn_710007a170(UnkAim* aim, f32 current);
UnkMatrix44 fn_7100078fc8(const UnkMatrix44& m);
void fn_71000774fc(UnkXform* x);
UnkMatrix44 fn_7100078834(const UnkKeyedEntry* entry, const UnkMatrix44* matrix, f32 weight);
