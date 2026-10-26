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
