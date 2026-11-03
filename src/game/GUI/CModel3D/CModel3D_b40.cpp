// GUI::CModel3D unit (tools/tu_map.py, the name from reflection): a reflected setter of the string
// id at +0x1a0 of the object at 8. A non-empty id that differs clears the word at +0x198, is stored
// and runs the virtual at 0x88. Returns the empty value. Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkModel3D_b40a3 {
    void** vtable;
    u8 pad_0x8[0x190];
    // A union member: the original reloads the id after clearing it.
    union {
        u32 field_0x198;
    };
    u8 pad_0x19c[4];
    CStrId model;  // 0x1a0
};

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71011f51d4(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    auto* target = *reinterpret_cast<UnkModel3D_b40a3**>(obj + 8);
    const CStrId* value = static_cast<const CStrId*>(ArgPtr_b40a3(arg));
    asm("" : "+r"(value));
    if (value->data != &g_unk_7101d47108->empty && target->model.data != value->data) {
        target->field_0x198 = 0;
        target->model = *value;
        reinterpret_cast<void (*)(UnkModel3D_b40a3*)>(target->vtable[0x88 / 8])(target);
    }
    return base::reflection::refl_EmptyValue();
}
