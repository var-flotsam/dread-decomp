// Unit 85: copies the reflected value stored under a key in a flat map, or returns the empty value.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkValueMap_l0e8044 {
    u8 pad_0x0[0x38];
    UnkReflValue* values;
    u64* keys;
    u32 count;

    UnkReflValue* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

UnkReflValue fn_71000e8044(UnkValueMap_l0e8044* self, const u64* key) {
    UnkReflValue* it = self->Find(*key);
    UnkReflValue r;
    if (it != self->values + self->count) {
        r.field_0x0 = nullptr;
        r.type = nullptr;
        r.kind = it->kind;
        const void* value = it->type == nullptr ? nullptr
                            : (it->field_0x14 == 1 || *reinterpret_cast<const u64*>(it->type + 0x18) > 8)
                                ? it->field_0x0
                                : it;
        fn_71000a18d4(&r, reinterpret_cast<const base::reflection::CType*>(it->type), value, false);
        return r;
    }
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
