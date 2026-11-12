// Unit 85 of tools/tu_map.py: resolves two typed values of an object (+0x18 and +0x28: an inline word
// and its type) that still hold a function's index (the 8-byte type, fn_7100010e58) into a pointer to
// that function of the class (CClass+0x110, 0xb8 bytes each; null for index -1), re-typed as the
// pointer type fn_710009ba1c: the old value destroyed (type +0x40), the new one copy-constructed (type
// +0x30). Names are placeholders.
#include "base/animation/CBlendSpaceResource.h"
#include "base/reflection/CClass.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;

const CType* fn_710009ba1c();  // a pointer type's getter

struct UnkTypedWord_b52 {
    u64 value;
    const CType* type;
};

struct UnkFunctionRefs_b52 {
    u8 pad_0x0[0x18];
    UnkTypedWord_b52 field_0x18;
    UnkTypedWord_b52 field_0x28;
};

static inline __attribute__((always_inline)) void Resolve(UnkTypedWord_b52* slot, CClass* cls) {
    if (slot->type != fn_7100010e58())
        return;
    s64 index = slot->value;
    void* fn = index != -1 ? static_cast<u8*>(cls->m_array_0x110.data) + (u64)(u32)index * 0xb8 : nullptr;
    const CType* type = fn_710009ba1c();
    if (slot->type) {
        slot->type->m_fn_0x40(slot);
        slot->type = nullptr;
    }
    if (type->m_fn_0x30) {
        // Stored as a plain pointer: the constructor is reloaded after this store.
        *reinterpret_cast<const CType**>(reinterpret_cast<u8*>(slot) + 8) = type;
        type->m_fn_0x30(slot, &fn);
    }
}

void fn_71000e7e54(UnkFunctionRefs_b52* self, CClass* cls) {
    Resolve(&self->field_0x18, cls);
    Resolve(&self->field_0x28, cls);
}
