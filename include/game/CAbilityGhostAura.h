#pragma once

// CAbilityGhostAura: functions and globals of this translation unit that other files use (tools/tu_map.py unit 430,
// 0x7100306910-0x71003162a8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct CGrappleBeamGun;
struct UnkAuraGunOwner;
struct UnkWeakHandle;
struct UnkWeakTarget;
struct UnkAbility;
struct UnkGhostAura_A2_b70;
struct UnkSelf;
u64 fn_7100308c74();
u32 fn_7100309568(UnkAbility* ability);
void fn_7100309fd8(UnkGhostAura_A2_b70* self, u32 a, u32 b);
bool fn_710030b8b4(UnkSelf* self, void* a2, void* zero);
extern void* g_unk_7101a22d48[];  // The listener's vtable.
extern void* g_unk_7101a22cb8[];
extern void* g_unk_7101a22d18[];
extern char g_unk_7101a22ce8[];
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
CGrappleBeamGun* fn_710030c5fc(UnkAuraGunOwner* self, const base::global::CStrId& id);
