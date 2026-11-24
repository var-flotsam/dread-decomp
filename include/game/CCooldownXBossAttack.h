#pragma once

// CCooldownXBossAttack: functions of this translation unit that other files call (tools/tu_map.py unit 745,
// 0x71004c1614-0x71004c2418). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCooldownXBossAttack_b62a2;
struct UnkLaserBite_b59;
struct UnkNode;
struct UnkNode_71004c3590;
struct UnkSelf;
struct UnkSelf_71004cb554;
struct UnkWhipSend_b58;
void fn_71004c1944(UnkSelf_71004cb554* self);
void fn_71004c1894(UnkNode_71004c3590*);
void fn_71004c196c(UnkCooldownXBossAttack_b62a2* self);

// Derived classes call it as their base's update (a thunk to fn_71003609a8).
void fn_71004c1968(UnkWhipSend_b58* self, f32 dt);

void fn_71004c1e08(UnkLaserBite_b59* self);
extern void* g_unk_7101a3e4b0[];  // CCooldownXBossLaserBiteAttack's vtable
extern void* g_unk_7101a3e8b8[];  // CCooldownXBossReaperAttack's vtable
extern void* g_unk_7101a3e5b8[];  // CCooldownXBossLavaCarpetAttack's vtable
