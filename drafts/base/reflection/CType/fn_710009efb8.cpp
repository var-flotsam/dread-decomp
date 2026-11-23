// base::reflection (unit 81): copy-assigns one reflected value from another, in place when the
// target already holds a compatible type, else by resetting and rebuilding it.
// Names and layouts are placeholders.
#include "base/UnkCloneCalls.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkTypeLinkTag;
using UnkTypeLinks = base::UnkKeyedArray<UnkTypeLinkTag, u64, void*>;

// The value's storage: inline when it fits in 8 bytes, else through field_0x0.
static inline const void* ValueData(const UnkReflValue* v) {
    const CType* type = reinterpret_cast<const CType*>(v->type);
    if (!type)
        return nullptr;
    if (v->field_0x14 == 1 || type->m_size > 8)
        return v->field_0x0;
    return v;
}

// fn_710009d52c, inlined here: `type` is `current`, derives from it, or is linked to it.
static inline bool IsKindOf(const CType* type, const CType* current) {
    for (const CType* t = type; t; t = t->m_parent) {
        if (t == current)
            return true;
    }
    u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(current->sName.data) + 0x30);
    do {
        auto* links = reinterpret_cast<UnkTypeLinks*>(const_cast<base::reflection::UnkReflArray*>(&type->m_array_0xb8));
        void** slot = links->Find(key);
        if (slot != links->End())
            return *slot != nullptr;
    } while ((type = type->m_parent));
    return false;
}

// Whether `self` can take a value of `type` stored at `data` without being rebuilt.
static inline bool CanAssignInPlace(const UnkReflValue* self, const CType* type, const void* data) {
    const CType* cur = reinterpret_cast<const CType*>(self->type);
    if (!cur || !data || self->field_0x14 != 0)
        return false;
    if (cur == type)
        return type->m_fn_0x48 != nullptr;
    return IsKindOf(type, cur);
}

UnkReflValue* fn_710009efb8(UnkReflValue* self, const UnkReflValue* other) {
    if (self == other)
        return self;
    const CType* type = reinterpret_cast<const CType*>(other->type);
    bool inPlace = CanAssignInPlace(self, type, ValueData(other));
    const void* data = ValueData(other);
    if (inPlace) {
        fn_71000a1cc8(self, type, data);
    } else {
        fn_71000a1c60(self);
        fn_71000a18d4(self, type, data, false);
    }
    return self;
}