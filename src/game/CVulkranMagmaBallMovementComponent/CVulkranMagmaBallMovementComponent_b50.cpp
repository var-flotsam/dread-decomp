// CVulkranMagmaBallMovementComponent unit (tools/tu_map.py). fn_71006df98c tells whether the
// owner's (+0x30) position, projected to 2D (fn_710007b430), is inside one of the registered
// objects (fn_71008fbdb4) of the class fn_71008fb79c names that isn't disabled (+0xcb1), by
// fn_7100a47e00. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassBaseTriggerComponent.h"
#include "game/CCharClassLavaPoolComponent.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkMagmaArea_b50 : UnkReflObject {
    u8 pad_0x8[0xca9];
    bool disabled;  // 0xcb1
};

struct UnkMagmaHandle_b50 {
    u8 pad_0x0[0x10];
    UnkReflObject* target;
};

struct UnkMagmaList_b50 {
    UnkMagmaHandle_b50** data;
    u32 count;
};

struct UnkMagmaOwner_b50 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkMagmaBall_b50 {
    u8 pad_0x0[0x30];
    UnkMagmaOwner_b50* owner;
};

bool fn_71006df98c(UnkMagmaBall_b50* self) {
    CVector2D point;
    {
        UnkMagmaOwner_b50* owner = self->owner;
        CVector3D pos;
        f32 x = owner->x;
        f32 y = owner->y;
        f32 z = owner->z;
        pos.z = z;
        pos.x = x;
        pos.y = y;
        fn_710007b430(&point, &pos);
    }
    UnkMagmaList_b50* list = fn_71008fbdb4();
    for (UnkMagmaHandle_b50 **it = list->data, **end = list->data + list->count; it != end; ++it) {
        UnkMagmaHandle_b50* handle = *it;
        if (!handle)
            continue;
        UnkReflObject* target = handle->target;
        if (!target || !fn_710009d50c(target->vf_0x0(), fn_71008fb79c()))
            continue;
        UnkMagmaArea_b50* area = static_cast<UnkMagmaArea_b50*>(target);
        if (area->disabled)
            continue;
        CVector2D copy = {point.x, point.y};
        if (fn_7100a47e00(area, &copy))
            return true;
    }
    return false;
}
