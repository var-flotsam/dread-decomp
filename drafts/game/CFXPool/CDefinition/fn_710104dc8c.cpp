// The CFXPool::CDefinition unit: takes an object from a pool of up to 32 (a bit per object in the
// mask at +0x38). With a free one it resets it (vf_0x118) and marks it used; when all are in use it
// reuses the one with the highest vf_0x78 value. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CPlasmaBeamGun.h"
#include "types.h"

struct UnkFxPoolEntry_l04dc8c {
    void** vtable;

    f32 vf_0x78() { return reinterpret_cast<f32 (*)(UnkFxPoolEntry_l04dc8c*)>(vtable[0x78 / 8])(this); }
    void vf_0x118() { reinterpret_cast<void (*)(UnkFxPoolEntry_l04dc8c*)>(vtable[0x118 / 8])(this); }
};

struct UnkFxObjectPool_l04dc8c {
    u8 pad_0x0[0x10];
    base::global::CRntVector<UnkFxPooledObject*> objects;  // 0x10
    u32 used;                                     // 0x38

};

static inline UnkFxPoolEntry_l04dc8c* At(base::global::CRntVector<UnkFxPooledObject*>* objects, u32 i) {
    return reinterpret_cast<UnkFxPoolEntry_l04dc8c*>(*fn_7100852548(objects, i));
}

UnkFxPoolEntry_l04dc8c* fn_710104dc8c(UnkFxObjectPool_l04dc8c* self) {
    base::global::CRntVector<UnkFxPooledObject*>* objects = &self->objects;
    UnkFxPoolEntry_l04dc8c* object = nullptr;
    if (self->used == 0xffffffff) {
        // All in use: reuse the one with the highest value.
        f32 best_value = -1.0f;
        for (u32 i = 0; i < objects->count; ++i) {
            if (!object || At(objects, i)->vf_0x78() > best_value) {
                object = At(objects, i);
                best_value = At(objects, i)->vf_0x78();
            }
        }
        if (object)
            object->vf_0x118();
    } else {
        for (u32 i = 0; i != objects->count; ++i) {
            u32 bit = 1u << i;
            if (!(self->used & bit)) {
                object = At(objects, i);
                object->vf_0x118();
                self->used |= bit;
                break;
            }
        }
    }
    return object;
}
