// The is-kind-of test of reflected types (declared in base/reflection/UnkReflCast.h): whether
// `type` is `base` or derives from it, walking the parent chain (CType::m_parent).
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/UnkCloneCalls.h"

bool fn_710009d50c(const base::reflection::CType* type, const base::reflection::CType* base) {
    for (; type; type = type->m_parent) {
        if (type == base)
            return true;
    }
    return false;
}

// The same test that also accepts a type `current` is registered under in a type's (or an ancestor's)
// array at +0xb8: values (non-null when the type takes it) keyed by the hash of a type's name.
struct UnkTypeLinkTag;
using UnkTypeLinks = base::UnkKeyedArray<UnkTypeLinkTag, u64, void*>;

bool fn_710009d52c(const base::reflection::CType* type, const base::reflection::CType* current) {
    for (const base::reflection::CType* t = type; t; t = t->m_parent) {
        if (t == current)
            return true;
    }
    u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(current->m_name.data) + 0x30);
    // The type is not checked for null again (the original reads a null type's array).
    do {
        auto* links = reinterpret_cast<UnkTypeLinks*>(const_cast<base::reflection::UnkReflArray*>(&type->m_array_0xb8));
        void** slot = links->Find(key);
        if (slot != links->End())
            return *slot != nullptr;
    } while ((type = type->m_parent));
    return false;
}
