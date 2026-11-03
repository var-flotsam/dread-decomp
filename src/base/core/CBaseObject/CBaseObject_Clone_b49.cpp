// base::core::CBaseObject's unit (unit 29 of tools/tu_map.py): a copy of an object through its
// reflected class (fn_71000238c8). When the object's dynamic class is a kind of
// base::core::CBaseObject (its class descriptor, g_unk_7101d4e990, inside the getter
// fn_7100023464's static) and has a constructor (+0x28), a block of the class's size (+0x18) is
// taken from the default allocator and constructed; then each 0xb0-byte field entry of the class
// (+0xe8) is copied over by fn_71000239a8 (also with no block). Names are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;

extern char g_unk_7101d4e990[] __attribute__((visibility("hidden")));

struct UnkClassField_b49a1 {
    u8 data[0xb0];
};

// The class descriptor's members used here.
struct UnkClassView_b49a1 {
    u8 unk_0x0[0x18];
    u64 size;                              // 0x18
    u8 unk_0x20[0x8];
    void (*construct)(void* obj);          // 0x28
    u8 unk_0x30[0xb8];
    UnkClassField_b49a1* fields;           // 0xe8
    void* field_keys;
    u32 field_count;                       // 0xf8
};

struct UnkBaseObject_b49a1 {
    virtual const CClass* vf_0x0() const;
};

void fn_71000239a8(void* dst, UnkBaseObject_b49a1* src, UnkClassField_b49a1* field, u32 flag);

void* fn_71000238c8(UnkBaseObject_b49a1* self, u32 flags) {
    const UnkClassView_b49a1* cls = reinterpret_cast<const UnkClassView_b49a1*>(self->vf_0x0());
    fn_7100023464();
    void* copy = nullptr;
    if (fn_710009d50c(reinterpret_cast<const CType*>(cls), reinterpret_cast<const CType*>(g_unk_7101d4e990)) &&
        cls->construct) {
        copy = fn_7100083990()->vf_0x40(cls->size, 8, flags);
        if (copy)
            cls->construct(copy);
    }
    u32 count = cls->field_count;
    if (count != 0) {
        UnkClassField_b49a1* field = cls->fields;
        for (UnkClassField_b49a1* end = field + count; field != end; ++field)
            fn_71000239a8(copy, self, field, 1);
    }
    return copy;
}
