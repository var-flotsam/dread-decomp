// Two helpers of reflected types (base::reflection::CType's unit, 0x710009c7b4): the dynamic
// type of an object through a descriptor's getter, and the assignment of a typed value into
// another type's value (declared in base/reflection/UnkReflTypedPtr.h and
// base/UnkCloneHelpers.h).
#include "base/UnkCloneHelpers.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflTypedPtr.h"

// The object's dynamic type through `type`'s getter (CType+0x68), null when it has none.
char* fn_710009dcd8(char* type, void* obj) {
    auto* t = reinterpret_cast<base::reflection::CType*>(type);
    if (t->m_fn_0x68)
        return reinterpret_cast<char*>(const_cast<base::reflection::CType*>(&t->m_fn_0x68(obj)));
    return nullptr;
}

namespace {
// A conversion registered for a type (the container at CType+0xb8: keys at +0xc0, values at
// +0xb8, count at +0xc8): a function at +0x10 taking the destination and source pointers.
struct UnkTypeConversion {
    u64 field_0x0;
    u64 field_0x8;
    void (*convert)(void* dst, void* src);
};

struct UnkConversionLookup {
    UnkTypeConversion** values;
    u64* keys;
    u32 count;

    UnkTypeConversion** End() { return &values[count]; }
    UnkTypeConversion** Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
};
}  // namespace

// NON_MATCHING(minor): same behaviour; the original tests the found slot against the end of the
// array without folding the miss case and lays the equal-type branch out last (16%).
bool fn_710009d7a8(const base::reflection::CType* type, base::reflection::UnkTypedRef* dst,
                   base::reflection::UnkTypedRef* src) {
    using namespace base::reflection;
    const CType* t = src->type;
    if (t != type) {
        u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(type->m_name.data) + 0x30);
        do {
            UnkConversionLookup a{static_cast<UnkTypeConversion**>(t->m_array_0xb8.data),
                                  static_cast<u64*>(t->m_array_0xb8.keys), t->m_array_0xb8.count};
            UnkTypeConversion** p = a.Find(key);
            if (p != a.End()) {
                if (!*p)
                    return false;
                (*p)->convert(dst->ptr, src->ptr);
                return true;
            }
            t = t->m_parent;
        } while (t);
        return false;
    }
    if (type->m_fn_0x48) {
        type->m_fn_0x48(dst->ptr, src->ptr);
        return true;
    }
    return false;
}

// ---- fn_710009ec00: registering a type ----

#include "base/reflection/UnkReflAttributes.h"

extern u32 g_unk_7101d5bdd8;    // set in editor builds: types get the "_EditorOnly" attribute
extern void* g_unk_7101d5bdc0;  // the registry's list of types (linked through CType+0x88)
using base::reflection::UnkReflAttrKey;
const UnkReflAttrKey& fn_710000d120();  // "_EditorOnly"

// The type's attribute container (CType+0x80), created on first use (fn_710009dd6c inlined).
struct UnkReflAttrsInit {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
    void* field_0x28;
    u32 field_0x30;
};
static inline base::reflection::UnkReflAttrs* AttrsOf(base::reflection::CType* type) {
    auto* attrs = static_cast<UnkReflAttrsInit*>(type->field_0x80);
    if (!attrs) {
        attrs = static_cast<UnkReflAttrsInit*>(fn_71000839a0(0x38, 0x3e));
        attrs->values = nullptr;
        attrs->keys = nullptr;
        attrs->count = 0;
        attrs->capacity = 0;
        attrs->step = 10;
        attrs->flags = 0x3e;
        attrs->allocator = fn_7100083990();
        attrs->field_0x28 = nullptr;
        attrs->field_0x30 = 0;
        type->field_0x80 = attrs;
    }
    return reinterpret_cast<base::reflection::UnkReflAttrs*>(attrs);
}

// Adds `type` to the front of the registry's list; in editor builds it is first marked "_EditorOnly".
void fn_710009ec00(void* list, void* object) {
    auto* type = static_cast<base::reflection::CType*>(object);
    if (g_unk_7101d5bdd8) {
        base::reflection::UnkReflAttrs* attrs = AttrsOf(type);
        const base::reflection::UnkReflAttrKey& key = fn_710000d120();
        bool value = true;
        attrs->Add(key, value);
    }
    if (g_unk_7101d5bdc0)
        type->field_0x88 = g_unk_7101d5bdc0;
    g_unk_7101d5bdc0 = type;
}
