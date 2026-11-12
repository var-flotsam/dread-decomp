// CActionSwitcherOnPullGrapplePointComponent's unit (tools/tu_map.py), fn_7100825f80: after fn_7100822888, finds
// the component's blackboard section (its id from fn_7100822a1c) in `blackboard`; when there is one, reads "Used"
// (g_unk_71727235d0) from it (fn_710006d11c) and, when that read fails, calls fn_710082cee4(1) and
// fn_710082cf08(0). Keys at +0x60 (count +0x68, values at +0x58), looked up as in
// src/game/CDoorLifeComponent/CDoorLifeComponent_b49.cpp. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CGrapplePointComponent.h"
#include "game/CPullableGrapplePointComponent.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkPullableGrapplePoint_b54;

struct UnkGrappleUsedValue_A4_b76 {
    CStrId id;
    u64 field_0x8;
};

struct UnkGrappleUsed_A4_b76 {
    u8 pad_0x0[0x58];
    UnkGrappleUsedValue_A4_b76* values;
    u64* keys;
    u32 count;

    __attribute__((always_inline)) UnkGrappleUsedValue_A4_b76* Find(const UnkProperty& property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* k = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        return &values[index];
    }
};

void fn_7100822888(UnkGrappleUsed_A4_b76* self, CBlackboard* blackboard);
CStrId fn_7100822a1c(UnkGrappleUsed_A4_b76* self);

void fn_7100825f80(UnkGrappleUsed_A4_b76* self, CBlackboard* blackboard) {
    fn_7100822888(self, blackboard);
    CBlackboard::CSection* s = fn_71010472e0(blackboard, fn_7100822a1c(self));
    if (!s)
        return;
    bool value = false;
    if (fn_710006d11c(s, &self->Find(g_unk_71727235d0)->id, &value))
        return;
    fn_710082cee4(reinterpret_cast<UnkPullableGrapplePoint_b54*>(self), true);
    fn_710082cf08(reinterpret_cast<UnkPullableGrapplePoint_b54*>(self), false);
}
