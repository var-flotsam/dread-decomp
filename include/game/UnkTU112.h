#pragma once

// UnkTU112: functions of this translation unit that other files call (tools/tu_map.py unit 112,
// 0x71001395b8-0x7100140988). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct UnkTunableLine;
struct UnkEntry_710013c3c8;
struct UnkTunableObject;
struct UnkTunableRegistry;
UnkTunableObject* fn_710013c5cc(UnkTunableRegistry* self, const char* name, bool qualified);
void fn_710013ffbc(UnkEntry_710013c3c8*, void*);
// A copy of `str` with every `from` replaced by `to`.
UnkTunableLine fn_710013c834(const base::global::CRntString* str, const char* from, const char* to);
