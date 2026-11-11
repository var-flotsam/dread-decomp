// CActor unit (tools/tu_map.py, hint from reflection): a reflected getter, the same code as
// fn_71010e791c (CCameraComponent): the entry stored under the id given (parallel arrays of values at
// +0xa0 and keys at +0xa8, count at +0xb0 of the object at 8), null when there is none, described by
// fn_710023e498. Names are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkReflType;

namespace {

struct UnkEntries_b41a1 {
    u8 pad_0x0[0xa0];
    void** values;
    u64* keys;
    u32 count;
};

inline __attribute__((always_inline)) void* ArgPtr_b41a1(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

inline void** Find_b41a1(UnkEntries_b41a1* self, const u64* id) {
    for (u64* k = self->keys; k != self->keys + self->count; ++k) {
        if (*k == *id)
            return self->values + (k - self->keys);
    }
    return self->values + self->count;
}

}  // namespace

UnkReflValue fn_710023e7cc(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* target = *reinterpret_cast<UnkEntries_b41a1**>(obj + 8);
    const u64* id = static_cast<const u64*>(ArgPtr_b41a1(a));
    void** it = Find_b41a1(target, id);
    void* v = it == target->values + target->count ? nullptr : *it;
    const auto* type = reinterpret_cast<const base::reflection::CType*>(fn_710023e498());
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}
