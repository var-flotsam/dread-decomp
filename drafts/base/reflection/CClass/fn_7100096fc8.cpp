// base::reflection::CClass: the types a class refers to (vtable slot 0x30): the base's (fn_710009d020),
// then each field's type, only the persistent fields' when `persistentOnly` is set. Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/CType.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

void fn_710009d020(const CType* self, base::global::CRntVector<const CType*>* out);
UnkReflValue fn_710000cf10(void* field, const base::reflection::UnkReflAttrKey& key, bool inherited);

using UnkRefsFn_l096fc8 = void (*)(const CType*, base::global::CRntVector<const CType*>*, bool);

struct UnkClassField_l096fc8 {
    u8 pad_0x0[0x10];
    const CType* type;  // 0x10
    u8 pad_0x18[0x98];
};
static_assert(sizeof(UnkClassField_l096fc8) == 0xb0, "field size");

struct UnkClass_l096fc8 {
    u8 pad_0x0[0xe8];
    UnkClassField_l096fc8* fields;  // 0xe8
    u8 pad_0xf0[0x8];
    u32 count;  // 0xf8
};

// Whether the field persists: when it has no "Persistence" attribute, or the attribute is 1.
static inline bool Persistent(UnkClassField_l096fc8* f) {
    UnkReflValue v = fn_710000cf10(f, fn_710000bf80(), true);
    if (!v.type) {
        fn_71000a1c60(&v);
        return true;
    }
    const void* p = v.field_0x14 == 1 || *reinterpret_cast<const u64*>(v.type + 0x18) >= 9 ? v.field_0x0 : &v;
    s32 value = *static_cast<const s32*>(p);
    fn_71000a1c60(&v);
    return value == 1;
}

void fn_7100096fc8(UnkClass_l096fc8* self, base::global::CRntVector<const CType*>* out, bool persistentOnly) {
    reinterpret_cast<UnkRefsFn_l096fc8>(&fn_710009d020)(reinterpret_cast<const CType*>(self), out, persistentOnly);
    UnkClassField_l096fc8* end = self->fields + self->count;
    for (UnkClassField_l096fc8* f = self->fields; f != end; ++f) {
        if (!persistentOnly || Persistent(f)) {
            const CType* type = f->type;
            out->PushBack(type);
        }
    }
}
