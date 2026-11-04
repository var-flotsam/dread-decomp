// CPlaythroughDef's unit (tools/tu_map.py unit 425): a reflected invoker (src/reflection/function_inits_00.cpp)
// returning the entry of the object's list (+0x68, count +0x70) whose id (+0x10) is the string id argument,
// or null, as a value of fn_71002f239c's pointer type. Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::refl_ArgValue;

struct UnkEnumType;
UnkEnumType* fn_71002f239c();

struct UnkPlaythroughEntry_A2_b71 {
    u8 pad_0x0[0x10];
    u64 id;  // 0x10
};

struct UnkPlaythroughList_A2_b71 {
    u8 pad_0x0[0x68];
    UnkPlaythroughEntry_A2_b71** data;  // 0x68
    u32 count;                          // 0x70
};

__attribute__((always_inline)) static inline UnkPlaythroughEntry_A2_b71* Find_A2_b71(
    UnkPlaythroughList_A2_b71* self, const CStrId* key) {
    for (UnkPlaythroughEntry_A2_b71 **p = self->data, **end = p + self->count; p != end; ++p) {
        if ((*p)->id == *reinterpret_cast<const u64*>(key))
            return *p;
    }
    return nullptr;
}

UnkReflValue fn_71002f466c(void*, char* obj, UnkReflValue* const* args) {
    auto* target = *reinterpret_cast<UnkPlaythroughList_A2_b71**>(obj + 8);
    UnkReflValue* arg = args[0];
    const CStrId* key = arg->type ? refl_ArgValue<CStrId>(arg) : nullptr;
    UnkPlaythroughEntry_A2_b71* found = Find_A2_b71(target, key);
    const CType* type = reinterpret_cast<const CType*>(fn_71002f239c());
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &found, false);
    return r;
}
