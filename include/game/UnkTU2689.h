#pragma once

// UnkTU2689: functions of this translation unit that other files call (tools/tu_map.py unit 2689,
// 0x71010d4ac0-0x71010def48). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

namespace base::reflection {
struct UnkTunable;
}
extern base::reflection::UnkTunable g_unk_7172a03d80;  // CPlayerMovement's tunable.

struct UnkMorphBallMovement_01d8;
struct UnkForcedMovementTarget_A5_b68;
struct UnkHitInfo_b152;
struct UnkMorphBallMovement_b49a4;
struct UnkMove_b152;
struct UnkPlayerMovementCache;
void fn_71010db850(UnkForcedMovementTarget_A5_b68* p, void* q);
void* fn_71010d7aac(UnkPlayerMovementCache* self);
void CPlayerMovement_Reset(UnkMorphBallMovement_b49a4* self, u32);
bool fn_71010d9638(UnkMove_b152* self, UnkHitInfo_b152* hit, u64* out);
void fn_71010db848(UnkForcedMovementTarget_A5_b68* self, const base::global::CStrId* name, const void* params);
void fn_71010d7874(void* p);
f32 fn_71010d6858();
void fn_71010d7b28(UnkMorphBallMovement_01d8* self, f32 dt);
