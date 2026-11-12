#pragma once

// CBlendSpaceResource: functions and globals of this translation unit that other files use (tools/tu_map.py unit 14,
// 0x710000928c-0x7100011758). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkVersionRegistry;
struct UnkFileReader;
bool fn_710000ccd8(UnkFileReader* self);
void* fn_710000ffd0();
// base::reflection's registry (0x1a8 bytes): the reflected types by 64-bit id.
extern UnkVersionRegistry* g_unk_7101d4c8d0;
extern char g_unk_7101d4c138[];
