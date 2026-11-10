// CPowerUpLifeComponent's unit (tools/tu_map.py): fn_71008e504c, a step after the life component's
// (fn_71008e0adc). With the pickable component (fn_71002a597c) of the object the handle at +0x3c8 refers
// to: on the first pass (state 1 at +0x3b8) the slot at +0x3c0 takes the handle fn_7100353be8 builds from
// +0x390 and the state becomes 2; when the slot's target has an object, that object's name (+0x30) is copied
// to the pickable's id at +0x120; then, when the player already has the item (fn_71009d377c), the entity at
// +0x30 is switched off (fn_7100232864). Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

using base::global::CStrId;

struct UnkEntityState;
void fn_7100232864(UnkEntityState* entity, bool value);

using UnkRef = base::UnkRefPtrMember;
UnkRef fn_7100353be8(void* source);

bool fn_71009d377c(char* self);

struct UnkRefl_fn_71002a597c;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_71002a597c>(void*);

struct UnkPowerUpLifeBase_ovb0126;
void fn_71008e0adc(UnkPowerUpLifeBase_ovb0126* self);

struct UnkPowerUpLifeHandle_ovb0126 {
    u8 pad_0x0[0x10];
    void* target;
};

struct UnkPowerUpLifeNamed_ovb0126 {
    u8 pad_0x0[0x30];
    CStrId name;
};

struct UnkPowerUpLifePickable_ovb0126 {
    u8 pad_0x0[0x120];
    CStrId id;
};

struct UnkPowerUpLifeStep_ovb0126 {
    u8 pad_0x0[0x30];
    UnkEntityState* entity;
    u8 pad_0x38[0x390 - 0x38];
    u8 field_0x390[0x28];
    u32 state;  // 0x3b8
    u8 pad_0x3bc[4];
    base::UnkRefHandle slot;  // 0x3c0
    UnkPowerUpLifeHandle_ovb0126* handle;  // 0x3c8
};

void fn_71008e504c(UnkPowerUpLifeStep_ovb0126* self) {
    fn_71008e0adc(reinterpret_cast<UnkPowerUpLifeBase_ovb0126*>(self));
    auto* pickable = reinterpret_cast<UnkPowerUpLifePickable_ovb0126*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_71002a597c>(self->handle ? self->handle->target : nullptr));
    if (!pickable)
        return;
    if (self->state == 1) {
        {
            base::UnkRefPtrMember handle = fn_7100353be8(self->field_0x390);
            fn_710031d208(&self->slot, reinterpret_cast<base::UnkRefHandle*>(&handle));
        }
        self->state = 2;
    }
    if (base::UnkRefCounted* target = self->slot.ptr) {
        if (*reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10)) {
            asm("" : "+r"(target));
            auto* named = *reinterpret_cast<UnkPowerUpLifeNamed_ovb0126**>(reinterpret_cast<char*>(target) + 0x10);
            CStrId name = named->name;
            pickable->id = name;
        }
    }
    if (fn_71009d377c(reinterpret_cast<char*>(pickable)))
        fn_7100232864(self->entity, false);
}
