// The CFXPool::CDefinition unit: takes the first free object of a pool of up to 32 (a bit per object
// in the mask at +0x38), resets it (vf_0x118) and marks it used; null when all are in use. The
// objects are in a growable array at +0x10 read through its out-of-line accessor fn_7100852548.
// Names and layouts are placeholders.
#include "base/UnkVector.h"

struct UnkFxPooledObject {
    void** vtable;

    void vf_0x118() { reinterpret_cast<void (*)(UnkFxPooledObject*)>(vtable[0x118 / 8])(this); }
};

struct UnkFxObjectPool {
    u8 pad_0x0[0x10];
    base::UnkVector<UnkFxPooledObject*> objects;  // 0x10
    u32 used;                                     // 0x38
};

// The element at `index`, growing the array to reach it (base::UnkVector's GrowAt, out of line).
UnkFxPooledObject** fn_7100852548(base::UnkVector<UnkFxPooledObject*>* array, u32 index);

UnkFxPooledObject* fn_710104dc00(UnkFxObjectPool* self) {
    if (self->used != 0xffffffff && self->objects.count != 0) {
        u32 i = 0;
        do {
            u32 bit = 1u << i;
            if (!(bit & self->used)) {
                UnkFxPooledObject* object = *fn_7100852548(&self->objects, i);
                object->vf_0x118();
                self->used |= bit;
                return object;
            }
            ++i;
        } while (i < self->objects.count);
    }
    return nullptr;
}
