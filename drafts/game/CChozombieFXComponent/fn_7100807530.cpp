// CChozombieFXComponent's unit (tools/tu_map.py unit 1270): the update. Counts down each of the ten effect
// slots that is running and, when one runs out, stops its model, marks it idle and resets its delay and
// direction, then hands over to fn_7100807998. Names and layouts are placeholders.
#include <new>

#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CComponent.h"
#include "game/UnkTU127.h"
#include "game/UnkTU55.h"
#include "types.h"

extern const base::math::CVector2D g_unk_7101647780;

// One effect slot.
struct UnkChozombieFxSlot_l807530 {
    UnkModel* model;
    base::math::CVector3D field_0x8;
    base::math::CVector3D field_0x14;
    base::math::CVector2D direction;  // 0x20
    f32 timer;                        // 0x28
    u8 pad_0x2c[4];
    u64 field_0x30;
    bool idle;  // 0x38
    u8 pad_0x39[7];

    UnkChozombieFxSlot_l807530()
        : model(nullptr),
          field_0x8{g_unk_710164779c.x, g_unk_710164779c.y, g_unk_710164779c.z},
          field_0x14{g_unk_710164779c.x, g_unk_710164779c.y, g_unk_710164779c.z},
          direction{g_unk_7101647780.x, g_unk_7101647780.y},
          timer(0.0f), field_0x30(0), idle(true) {}
};
static_assert(sizeof(UnkChozombieFxSlot_l807530) == 0x40, "layout");

// A fixed array over raw storage that grows on access: reaching past the end constructs the slots up to the
// one read.
template <class T, u32 N>
struct UnkGrowArray_l807530 {
    u32 count;
    alignas(4) u8 storage[N * sizeof(T)];

    T* Items() { return reinterpret_cast<T*>(storage); }
    T& operator[](u64 i) {
        if (i >= count) {
            for (u64 n = count; n <= i; ++n)
                new (&Items()[n]) T();
            count = i + 1;
        }
        return Items()[i];
    }
};

struct UnkChozombieFxData_l807530 {
    u8 pad_0x0[0x70];
    f32 delay;  // 0x70
};

struct UnkChozombieFX_l807530 {
    u8 pad_0x0[0x10];
    UnkChozombieFxData_l807530* data;  // 0x10
    u8 pad_0x18[0x50 - 0x18];
    UnkGrowArray_l807530<UnkChozombieFxSlot_l807530, 10> slots;  // 0x50
};
static_assert(__builtin_offsetof(UnkChozombieFX_l807530, slots) == 0x50, "layout");

void fn_7100807998(UnkChozombieFX_l807530* self);

void fn_7100807530(UnkChozombieFX_l807530* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    for (u64 i = 0; i < 10; ++i) {
        if (self->slots[i].idle)
            continue;
        self->slots[i].timer -= dt;
        if (self->slots[i].timer > 0.0f)
            continue;
        fn_71001436a0(self->slots[i].model, 0);
        fn_710014364c(self->slots[i].model, 0);
        self->slots[i].idle = true;
        self->slots[i].timer = self->data->delay;
        self->slots[i].field_0x30 = 0;
        fn_710007b444(&self->slots[i].direction, reinterpret_cast<const UnkVec3*>(&g_unk_710164779c));
    }
    fn_7100807998(self);
}
