// base::reflection::CClass: the state checksum of a class: its id, then the checksum of every field
// whose "Persistence" attribute is missing or 1. Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/reflection/CType.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkHashed_71000e81e8;
s32 ComputeClassFieldChecksum(UnkHashed_71000e81e8* self);

struct UnkClassField_0xb0 {
    u8 pad_0x0[0xb0];
};

struct UnkClass_l097250 {
    u8 pad_0x0[0xe8];
    UnkClassField_0xb0* fields;  // 0xe8
    u8 pad_0xf0[0x8];
    u32 count;  // 0xf8
};

// Whether the field takes part: when it has no such attribute, or the attribute is 1.
static inline bool Persistent(UnkClassField_0xb0* f) {
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

s32 fn_7100097250(UnkClass_l097250* self) {
    UnkScopedHashStream s(0x16, nullptr);
    s.Put(CType_ComputeBaseHash(reinterpret_cast<UnkHashed*>(self)));
    UnkClassField_0xb0* end = self->fields + self->count;
    for (UnkClassField_0xb0* f = self->fields; f != end; ++f) {
        if (Persistent(f))
            s.Put(ComputeClassFieldChecksum(reinterpret_cast<UnkHashed_71000e81e8*>(f)));
    }
    return s.Finish();
}
