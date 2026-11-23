#pragma once

#include "base/math/CVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkChargeEntry_ovb0016 {
    base::math::CVector3D pos;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    UnkRefObject* ref;

    UnkChargeEntry_ovb0016(const UnkChargeEntry_ovb0016& o)
        : pos(o.pos), field_0xc(o.field_0xc), field_0x10(o.field_0x10), field_0x14(o.field_0x14), field_0x18(o.field_0x18),
          field_0x1c(o.field_0x1c), ref(nullptr) {
        AssignRef(&ref, o.ref);
    }
    ~UnkChargeEntry_ovb0016() {
        if (UnkRefObject* p = ref) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};
