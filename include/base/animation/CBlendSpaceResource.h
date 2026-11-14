#pragma once

// CBlendSpaceResource: functions and globals of this translation unit that other files use (tools/tu_map.py unit 14,
// 0x710000928c-0x7100011758). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkParseContext;
struct UnkVersionRegistry;
struct UnkFileReader;
bool fn_710000ccd8(UnkFileReader* self);
void* fn_710000ffd0();
// base::reflection's registry (0x1a8 bytes): the reflected types by 64-bit id.
extern UnkVersionRegistry* g_unk_7101d4c8d0;
extern char g_unk_7101d4c138[];
void fn_710000f750(UnkParseContext* self, void* root);
const base::reflection::UnkReflAttrKey& fn_710000d120();
const base::reflection::CType* fn_7100010e58();  // An 8-byte type's getter.
extern const base::reflection::CType g_unk_7101d4cfb0 __attribute__((visibility("hidden")));
