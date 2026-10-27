// From unit 70 of tools/tu_map.py (0x71000934b8-0x71000943bc; its initialiser,
// src/game_s1/fn_7100094300.cpp, registers the "base::reflection::EAttributeTarget" enum): a
// reflected element's attributes (UnkReflAttrs, base/reflection/UnkReflAttributes.h), values keyed
// by the attribute key's name, and the insertion every attribute registration goes through
// (fn_71000934b8). Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflAttrKey;
using base::reflection::UnkReflAttrs;
using base::reflection::UnkReflValue;
using base::reflection::UnkReflValueRef;

// The attribute registry (GOT-reached): the known attribute keys by name in a keyed array at +0xc0
// of 0x18-byte records, looked up by a shared out-of-line copy (src/clones/).
struct UnkVersionRegistry;
extern UnkVersionRegistry* g_unk_7101d4c8d0;
struct UnkClone_fn_71000a53c0;
extern template void* base::refl_KeyedFind<UnkClone_fn_71000a53c0, 0xc0, 0x18>(char*, const base::UnkStrIdMember*);

// A known attribute: the type its values must have (null: any) and the targets it applies to (a bit
// per target); a set flag at +0x14 refuses every value.
struct UnkReflAttrInfo {
    u8 pad_0x0[0x8];
    const CType* type;
    u32 targets;
    bool refused;
};

const UnkReflAttrKey& fn_710000d120();  // "_EditorOnly"
// A value converted to `type` (returned through x8).
UnkReflValue fn_71000a1774(const UnkReflValueRef* value, const CType* type);
void fn_71000a179c(UnkReflValue* self, const UnkReflValue* other);  // copy constructor

// UnkReflAttrs's layout: values and their keys' names side by side (CBlackboard.h's
// UnkPropertyArrays), the type a value gets without a known attribute, and the target the element
// is (an EAttributeTarget).
struct UnkReflAttrsMap {
    UnkReflValue* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
    const CType* default_type;
    u32 target;

    UnkReflValue* End() { return &values[count]; }
    // The value named `key`, or End().
    UnkReflValue* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return End();
    }
};
static_assert(sizeof(UnkReflAttrsMap) == 0x38, "UnkReflAttrs layout");

// Whether a new value of `value`'s type may be added under `key`.
static inline bool CanAdd(const UnkReflAttrsMap* attrs, const UnkReflAttrKey& key, const UnkReflValueRef& value,
                          const UnkReflAttrInfo* info, bool known) {
    if (!g_unk_7101d4c8d0 && key.name.data == fn_710000d120().name.data)
        return true;
    if (!known)
        return false;
    bool ok;
    if (info->refused)
        ok = !info->refused;
    else
        ok = !info->type || fn_710009d52c(value.type, info->type);
    if (!ok)
        return false;
    return (info->targets & (1u << attrs->target)) != 0;
}

// Adds an empty value under `key`; the new value (the first one when the count wraps).
static inline UnkReflValue* Insert(UnkReflAttrsMap* attrs, const UnkReflAttrKey& key) {
    UnkReflValue empty;
    empty.field_0x0 = nullptr;
    empty.type = nullptr;
    empty.kind = 2;
    empty.field_0x14 = 1;
    if (attrs->count == attrs->capacity)
        fn_7100093dd0(reinterpret_cast<UnkPropertyArrays*>(attrs), attrs->step + attrs->count);
    new (&attrs->keys[attrs->count]) CStrId(key.name);
    fn_71000a179c(&attrs->values[attrs->count], &empty);
    u32 n = ++attrs->count;
    UnkReflValue* values = attrs->values;
    fn_71000a1c60(&empty);
    return &values[n ? n - 1 : 0];
}

// Stores `value` in `slot`, converted to `type` when that differs and can be copied.
static inline void SetValue(UnkReflValue* slot, const CType* type, const UnkReflValueRef& value) {
    const CType* from = value.type;
    if (type && from != type && type->m_fn_0x30) {
        UnkReflValue converted = fn_71000a1774(&value, type);
        const CType* t = reinterpret_cast<const CType*>(converted.type);
        const void* p = nullptr;
        if (t)
            p = converted.field_0x14 == 1 || t->m_size >= 9 ? converted.field_0x0 : &converted;
        fn_71000a1c60(slot);
        fn_71000a18d4(slot, t, p, 0);
        fn_71000a1c60(&converted);
    } else {
        // Read before the destroy, which writes and frees the slot's data.
        const void* p = value.value;
        fn_71000a1c60(slot);
        fn_71000a18d4(slot, from, p, 0);
    }
}

// Adds `value` under `key`, or replaces the value already there. A new key is taken only when the
// registry knows it, it accepts the value's type and applies to this element's target (without a
// registry, "_EditorOnly" alone). The value is stored converted to the attribute's type when that
// differs and can be copied.
// NON_MATCHING(minor): block layout and registers: the original falls through to the new-key checks
// after the lookup (here the existing-key path does), keeps the attributes in x21 and the type in
// x20 (swapped here), and tests the "refused" byte as (byte ^ 1) where this tests it for nonzero
// (the same for a bool); the same calls with the same arguments (752 B original, 708 B here).
void fn_71000934b8(UnkReflAttrs* attrs_, const UnkReflAttrKey& key, const UnkReflValueRef& value) {
    auto* attrs = reinterpret_cast<UnkReflAttrsMap*>(attrs_);
    UnkReflAttrInfo* info = nullptr;
    bool known = false;
    if (g_unk_7101d4c8d0) {
        info = static_cast<UnkReflAttrInfo*>(base::refl_KeyedFind<UnkClone_fn_71000a53c0, 0xc0, 0x18>(
            reinterpret_cast<char*>(g_unk_7101d4c8d0), reinterpret_cast<const base::UnkStrIdMember*>(&key.name)));
        known = info != nullptr;
    }
    const CType* type = known && info->type ? info->type : attrs->default_type;
    UnkReflValue* slot = attrs->Find(key.name);
    if (slot == attrs->End()) {
        if (!CanAdd(attrs, key, value, info, known))
            return;
        slot = Insert(attrs, key);
    }
    if (slot)
        SetValue(slot, type, value);
}
