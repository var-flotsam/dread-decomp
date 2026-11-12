#pragma once

// CTunableShotManager: functions of this translation unit that other files call (tools/tu_map.py unit 2576,
// 0x7100ecfb5c-0x7100edf654). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkHyperBeamSoundHandle;
struct UnkObj290_b43a3;
struct UnkSlot_0x40;
void fn_7100ed78c0(void* player);
void fn_7100ed6af0(void* manager, u32 id, u32 slot);
void fn_7100ed8144(void* it);
void fn_7100ed977c(UnkSlot_0x40* self);  // Destructor.

// Assignment (its callers use it as copy and as move assignment).
void fn_7100edbef4(UnkSlot_0x40* self, UnkSlot_0x40* other);

void fn_7100eddc68(UnkObj290_b43a3* self, UnkObj290_b43a3* other);  // Move constructor.
UnkHyperBeamSoundHandle fn_7100ed6910(void* player, const base::math::CVector3D* pos, u32 id, u32 kind, u32);
