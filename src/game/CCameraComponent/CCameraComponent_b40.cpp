// CCameraComponent unit (tools/tu_map.py, the name from reflection): a reflected function that passes
// its first two arguments (null when an argument has no type; inline, or behind a pointer for values
// over 8 bytes or kind 1) to fn_71010e655c on the first parameter and returns the int 0 as a reflected
// value; and a getter of the entry stored under the id given (parallel arrays of values at +0x80 and
// keys at +0x88, count at +0x90 of the component at 8), null when there is none, described by
// fn_71010df6c0. Names are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_71010e655c(char* self, void* a, void* b);

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71010e7860(char* self, void*, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = self;
    void* p0 = ArgPtr_b40a3(&a[0]);
    void* p1 = ArgPtr_b40a3(&a[1]);
    fn_71010e655c(target, p0, p1);
    s32 v = 0;
    const base::reflection::CType* type = fn_7100014768();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}

const base::reflection::CType* fn_71010df6c0();

struct UnkCameraEntries_b40a3 {
    u8 pad_0x0[0x80];
    void** values;
    u64* keys;
    u32 count;
};

static inline void** Find_b40a3(UnkCameraEntries_b40a3* self, const u64* id) {
    for (u64* k = self->keys; k != self->keys + self->count; ++k) {
        if (*k == *id)
            return self->values + (k - self->keys);
    }
    return self->values + self->count;
}

UnkReflValue fn_71010e791c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* target = *reinterpret_cast<UnkCameraEntries_b40a3**>(obj + 8);
    const u64* id = static_cast<const u64*>(ArgPtr_b40a3(a));
    void** it = Find_b40a3(target, id);
    void* v = it == target->values + target->count ? nullptr : *it;
    const base::reflection::CType* type = fn_71010df6c0();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}
