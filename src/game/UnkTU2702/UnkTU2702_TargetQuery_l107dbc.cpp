// Unit 2702 (hint CAIComponent): asks the component's slot 0x330 about the owner's target actor's
// position (2D, replaced by its tracked entry's point when it has a live one); FLT_MAX when there is no
// target. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkTrackedShape_l107dbc {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkTrackedEntry_l107dbc {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
    u8 pad_0xfa[0x2e];
    UnkTrackedShape_l107dbc* field_0x128;
    u8 pad_0x130[0xc];
    u8 field_0x13c[1];
};

struct UnkTargetActor_l107dbc {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x188];
    UnkTrackedEntry_l107dbc* field_0x1f0;
};

struct UnkQuerySelf_l107dbc {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30

    f32 vf_0x330(const CVector2D* point, bool a, bool b) {
        return reinterpret_cast<f32 (*)(UnkQuerySelf_l107dbc*, const CVector2D*, bool, bool)>(vtable[0x330 / 8])(this, point, a, b);
    }
};

static inline CVector3D Position_l107dbc(const UnkTargetActor_l107dbc* actor) {
    CVector3D r;
    r.x = actor->x;
    r.y = actor->y;
    r.z = actor->z;
    return r;
}

f32 fn_7101107dbc(UnkQuerySelf_l107dbc* self, bool a, bool b) {
    UnkTargetActor_l107dbc* target = reinterpret_cast<UnkTargetActor_l107dbc*>(fn_7100d7c990(self->owner));
    if (target == nullptr)
        return 3.4028235e38f;
    CVector2D point;
    {
        CVector3D v = Position_l107dbc(target);
        fn_710007b430(&point, &v);
    }
    UnkTrackedEntry_l107dbc* entry = target->field_0x1f0;
    if (entry && !entry->field_0xf9 && entry->field_0x128 && entry->field_0x128->field_0x10)
        fn_710007b444(&point, reinterpret_cast<const UnkVec3*>(entry->field_0x13c));
    return self->vf_0x330(&point, a, b);
}
