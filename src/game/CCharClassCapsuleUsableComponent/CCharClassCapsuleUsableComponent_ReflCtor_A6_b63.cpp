// CCharClassCapsuleUsableComponent's unit: the reflected class's construct-in-place slot (0x28 of
// src/reflection/CCharClassCapsuleUsableComponent.cpp), the shape of fn_7100254584
// (src/game/CActorComponentDef/CActorComponentDef_ReflCtor_b59.cpp): the base constructor (fn_7100a7ff04), the
// vtable, three string ids (+0x1d8, +0x1e0, +0x1e8) set to the empty id, then the object's own vtable slot 3.
// Names are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

extern char g_unk_7101a90b30[];
void fn_7100a7ff04(void* self);

struct UnkCapsuleUsableDef_A6_b63 {
    void* vtable;
    u8 pad_0x8[0x1d0];
    CStrId id_0x1d8;
    CStrId id_0x1e0;
    CStrId id_0x1e8;
};

static inline void CopyId_A6_b63(CStrId* id, UnkStringData* data) {
    id->data = data;
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

void fn_7100a79630(void* obj) {
    UnkCapsuleUsableDef_A6_b63* self = static_cast<UnkCapsuleUsableDef_A6_b63*>(obj);
    fn_7100a7ff04(self);
    self->vtable = g_unk_7101a90b30 + 0x10;
    UnkStringData* empty = g_unk_7101d470e0.data;
    CopyId_A6_b63(&self->id_0x1d8, empty);
    CopyId_A6_b63(&self->id_0x1e0, empty);
    CopyId_A6_b63(&self->id_0x1e8, empty);
    // The vtable is read back as any type could have changed it (after a count update).
    typedef void (*Slot)(void*);
    typedef Slot* __attribute__((may_alias)) VtablePtr;
    VtablePtr vtable = *reinterpret_cast<VtablePtr*>(self);
    vtable[3](self);
}
