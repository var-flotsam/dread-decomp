#pragma once

// CCooldownXBossAttack: functions of this translation unit that other files call (tools/tu_map.py unit 745,
// 0x71004c1614-0x71004c2418). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCooldownXBossAttack_b62a2;
struct UnkLaserBite_b59;
struct UnkNode;
struct UnkSelf;
struct UnkWhipSend_b58;
void fn_71004c1944(UnkSelf* self);
void fn_71004c1894(UnkNode*);
void fn_71004c196c(UnkCooldownXBossAttack_b62a2* self);
void fn_71004c1968(UnkWhipSend_b58* self, f32 dt);
void fn_71004c1e08(UnkLaserBite_b59* self);
