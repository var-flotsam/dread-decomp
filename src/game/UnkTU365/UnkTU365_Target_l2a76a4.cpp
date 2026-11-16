// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator): the slot holding a breakable part's target,
// resolved on first use: the weak handle of the actor its reflected property names, kept from then on.
// Names and layouts are placeholders.
#include "game/UnkTU365.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkBreakablePartLayout_l2a76a4 {
    u8 pad_0x0[0x8];
    u8 property[0x28];  // 0x8, read through fn_710023b580
    u32 state;  // 0x30: 0 none, 1 to resolve, 2 resolved
    UnkWeakHandle* target;  // 0x38
};

UnkBreakableTarget** fn_71002a76a4(UnkBreakablePart* part) {
    auto* self = reinterpret_cast<UnkBreakablePartLayout_l2a76a4*>(part);
    switch (self->state) {
    case 0:
        return reinterpret_cast<UnkBreakableTarget**>(&self->target);
    case 1: {
        UnkRefPtr ref = fn_710023b580(&self->property);
        if (static_cast<void*>(&self->target) != &ref)
            CopyWeakRef(&self->target, static_cast<UnkWeakHandle*>(ref.field_0x0));
    }
        self->state = 2;
        return reinterpret_cast<UnkBreakableTarget**>(&self->target);
    case 2:
        return reinterpret_cast<UnkBreakableTarget**>(&self->target);
    }
    return reinterpret_cast<UnkBreakableTarget**>(&self->target);
}
