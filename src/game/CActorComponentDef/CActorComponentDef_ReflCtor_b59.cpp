// From unit 343 (CActorComponentDef): the reflected class's construct-in-place slot (0x28 of
// src/reflection/CActorComponentDef.cpp): the constructor CActorComponentDef_Ctor.cpp matches
// (fn_71002536b0, inlined here), then the object's own vtable slot 3. Names are placeholders.
#include "base/core/CBaseObject.h"
#include "base/global/CStrId.h"
#include "game/CActorComponentDef.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;


struct UnkComponentDefEntry_b59 {
    void* vtable;
    CStrId id_0x8;
    CStrId id_0x10;
    u64 field_0x18;
    bool flag_0x20;
    bool flag_0x21;
    void* field_0x28;
};

static inline void CopyId(CStrId* id, UnkStringData* data) {
    id->data = data;
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

void fn_7100254584(void* obj) {
    UnkComponentDefEntry_b59* self = static_cast<UnkComponentDefEntry_b59*>(obj);
    fn_7100023838(self);
    self->vtable = g_unk_7101a20ba8 + 0x10;
    UnkStringData* empty = g_unk_7101d470e0.data;
    CopyId(&self->id_0x8, empty);
    CopyId(&self->id_0x10, empty);
    self->field_0x18 = 0;
    self->flag_0x20 = true;
    self->flag_0x21 = true;
    self->field_0x28 = nullptr;
    // The vtable is read back as any type could have changed it (after a count update).
    typedef void (*Slot)(void*);
    typedef Slot* __attribute__((may_alias)) VtablePtr;
    VtablePtr vtable = *reinterpret_cast<VtablePtr*>(self);
    vtable[3](self);
}
