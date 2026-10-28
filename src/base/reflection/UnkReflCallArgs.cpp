// From unit 94 of tools/tu_map.py (0x71000efdb0-0x71000f3410, no class hint): binding the arguments
// of a reflected call. A callee's parameters are listed by name and type (fn_710009b050, the same
// list an enum's values give); the first may be the index or key ("uIdx" or "sKey", by the caller's
// mode) and the next the object's type ("oType"). Names and layouts are placeholders.
// Holds fn_71000f01d0.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

// The parameter list, returned in memory (src/base/reflection/UnkEnumValueNames.cpp's layout):
// each parameter's name and type.
struct UnkEnumType;
struct UnkReflParam {
    CStrId name;
    const base::reflection::CType* type;
};
struct UnkEnumNames : base::UnkVector<UnkReflParam> {
    UnkEnumNames(const UnkEnumNames& other);
};
UnkEnumNames fn_710009b050(const UnkEnumType* type);

// Appends a bound argument to the call.
void fn_71000f3030(void* call, UnkReflValue* value);

// The caller's mode at +0xe0: 1 binds by index.
struct UnkReflCallOwner {
    u8 pad_0x0[0xe0];
    s32 mode;
};

// A reflected object: its slot 0 gives its type.
struct UnkReflSelf {
    virtual const base::reflection::CType* vf_0x0();
};

static inline void Bind(void* call, const base::reflection::CType* type, const void* value, u32 kind) {
    UnkReflValue v;
    v.field_0x0 = nullptr;
    v.type = nullptr;
    v.kind = kind;
    fn_71000a18d4(&v, type, value, 1);
    fn_71000f3030(call, &v);
    fn_71000a1c60(&v);
}

// Binds the leading parameters of `callee` the call can fill: the index or key from `key` (when the
// first parameter has that name and type) and then the object `self` (when the next is "oType" of
// type CType). Returns how many were bound.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
u64 fn_71000f01d0(void* call, UnkReflCallOwner* owner, void*, const UnkEnumType* callee, const UnkReflValue* key,
                  UnkReflSelf* self) {
    static CStrId s_index("uIdx", true);
    static CStrId s_key("sKey", true);
    static CStrId s_type("oType", true);
    UnkEnumNames params = fn_710009b050(callee);
    u64 bound = 0;
    if (params.count != 0) {
        const CStrId& first = owner->mode == 1 ? s_index : s_key;
        const base::reflection::CType* type = params.data[0].type;
        if (params.data[0].name.data == first.data && type == reinterpret_cast<const base::reflection::CType*>(key->type)) {
            const void* value = nullptr;
            if (type)
                value = (key->field_0x14 == 1 || *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(type) + 0x18) > 8)
                            ? key->field_0x0
                            : static_cast<const void*>(key);
            bound = 1;
            Bind(call, type, value, key->kind);
            if (params.count < 2)
                goto done;
        }
        if (params.data[bound].name.data == s_type.data &&
            params.data[bound].type == reinterpret_cast<const base::reflection::CType*>(fn_710009c7b8())) {
            Bind(call, self->vf_0x0(), self, 2);
            ++bound;
        }
    }
done:
    if (params.data) {
        for (u64 i = 0; i < params.count; ++i)
            params.data[i].name.Reset();
        params.count = 0;
        params.allocator->vf_0x48(params.data);
    }
    return bound;
}
