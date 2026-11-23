// Unit 93 of tools/tu_map.py: the text writer for a string id: converts the typed value to a
// base::global::CStrId and copies its text into the writer's 0x200-byte buffer at +0xc; false
// when the value does not convert. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkCloneHelpers.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

struct UnkTextWriter_b0166 {
    u8 unk_0x0[0xc];
    char text[0x200];
};

// src/base/reflection/CType/CType_Helpers_ovb0053.cpp.
UnkReflValue fn_710009da40(const CType* type, const UnkTypedRef* src, const CType* to);

// Copies the value into `out` when it holds a string id.
static inline bool GetStrId(UnkReflValue* value, CStrId& out) {
    const CType* type = reinterpret_cast<const CType*>(value->type);
    if (!type || !fn_710009d50c(type, fn_7100009658()))
        return false;
    out = *base::reflection::refl_ArgValue<CStrId>(value);
    return true;
}

// Releases a reflected value when it goes out of scope.
struct UnkReflValueRelease_l0ef534 {
    UnkReflValue* value;
    ~UnkReflValueRelease_l0ef534() { fn_71000a1c60(value); }
};

bool fn_71000ef534(UnkTextWriter_b0166* self, const UnkTypedRef* value) {
    const CType* type = value->type;
    UnkTypedRef ref;
    fn_71000a174c(&ref, value);
    UnkReflValue converted = fn_710009da40(type, &ref, fn_7100009658());
    UnkReflValueRelease_l0ef534 release = {&converted};
    if (!converted.type)
        return false;
    CStrId id(base::global::UnkStrIdInline(), nullptr, false);
    if (!GetStrId(&converted, id))
        return false;
    strncpy(self->text, id.Text(), sizeof(self->text));
    return true;
}
