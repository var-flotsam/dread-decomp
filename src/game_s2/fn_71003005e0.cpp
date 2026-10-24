// Finds the component named `id` in the array at +0x40 (compared with the id of the record at +0xc8 of
// each entry) and returns it when it is an instance of the class fn_710033a31c describes, else null.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

struct UnkNamed {
    CStrId field_0x0;
};

struct UnkEntry {
    void** vtable;
    u8 pad_0x8[0xc0];
    UnkNamed* field_0xc8;
};

struct UnkOwner {
    u8 pad_0x0[0x40];
    UnkEntry** field_0x40;
    u32 field_0x48;
};

const CClass* fn_710033a31c();

// The first entry is tested before the loop, as in the original.
static inline UnkEntry** FindByName(UnkEntry** begin, UnkEntry** end, CStrId key) {
    UnkEntry** it = begin;
    if (it != end) {
        if ((*it)->field_0xc8->field_0x0.data == key.data)
            return it;
        for (++it; it != end; ++it)
            if ((*it)->field_0xc8->field_0x0.data == key.data)
                return it;
    }
    return end;
}

UnkEntry* fn_71003005e0(UnkOwner* self, const CStrId* id) {
    UnkEntry** it = FindByName(self->field_0x40, self->field_0x40 + self->field_0x48, *id);
    if (it == self->field_0x40 + self->field_0x48)
        return nullptr;
    UnkEntry* entry = *it;
    if (!entry)
        return nullptr;
    return fn_710009d50c(reinterpret_cast<const CClass* (*)(UnkEntry*)>(entry->vtable[0])(entry), fn_710033a31c()) ? entry : nullptr;
}
