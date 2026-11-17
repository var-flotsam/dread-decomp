#pragma once

// UnkTU211: functions and globals of this translation unit that other files use (tools/tu_map.py unit 211,
// 0x7100190154-0x7100194a9c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

struct UnkKeyPlayer_b0197;
struct UnkKeyRequest_b0197;
struct UnkEntryListA1_134;
struct UnkDecalResourceMgr;
extern UnkDecalResourceMgr* g_unk_716e352450;
bool fn_7100193410(UnkEntryListA1_134* list, u32 index, u32 slot, const base::math::UnkVector4D& value, u32 flag);
void fn_7100192f08(void* obj, bool flag);
void fn_71001933cc(void*);
u64 fn_710019199c(UnkKeyPlayer_b0197* self, const UnkKeyRequest_b0197* request, void* track);
u64 fn_7100191d40(UnkKeyPlayer_b0197* self, const UnkKeyRequest_b0197* request, void* track);
extern char g_unk_7101a1dca0[];
extern char g_unk_7101a1dcc8[];
extern char g_unk_7101a1dcf0[];
