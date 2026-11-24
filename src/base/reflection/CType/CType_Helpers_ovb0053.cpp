// base::reflection::CType's unit (tools/tu_map.py, unit 81): helpers of reflected type descriptors.
// A type's destructor (fn_710009cc04) and the release of its attribute container (fn_710009cee0);
// attribute lookup (fn_710009ddd4); the typed pointer of a held value (fn_710009ee40); assignment,
// conversion and comparison of typed values through a type's functions and its conversion table at
// +0xb8 (fn_710009d960, fn_710009da40, fn_710009d5d4); a type's parent and the types it refers to
// (fn_710009d020, fn_710009d1ec); the "AssetHash" attribute (fn_710009ea3c). Names and layouts
// are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CType.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflAttrKey;
using base::reflection::UnkReflAttrs;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

extern void* g_unk_7101a1b540[];  // CType's vtable
const UnkReflAttrKey& fn_71000905e0();  // "AssetHash"

namespace {
// The type's virtual interface as far as it is used here: slot 0x30 lists the types it refers to,
// slot 0x38 computes the type's own hash.
struct UnkTypeVirtuals_ovb0053 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(base::global::CRntVector<CType*>* out, bool flag);
    virtual s32 vf_0x38();
};

// The type's attribute container (CType+0x80): values, string-id keys, count, and the allocator.
using UnkTypeAttrs_ovb0053 = base::global::CRntSmallDictionary<CStrId, UnkReflValue>;

// The same container as fn_710009dd6c creates it on first use.
struct UnkReflAttrsInit_ovb0053 {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;
    void* field_0x28;
    u32 field_0x30;
};

// Its lookup by key.
struct UnkAttrTable_ovb0053 {
    UnkReflValue* values;
    const void* const* keys;
    u32 count;

    UnkReflValue* Find(const UnkReflAttrKey& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key.name.data)
                return &values[i];
        return &values[count];
    }
};

// A conversion registered for a type (the container at CType+0xb8: values at +0xb8, keys at +0xc0,
// count at +0xc8), keyed by the hash of the target type's name.
struct UnkTypeConversion_ovb0053 {
    u64 field_0x0;
    u64 field_0x8;
    void (*convert)(void* dst, void* src);
};

struct UnkConversionTable_ovb0053 {
    UnkTypeConversion_ovb0053** values;
    const u64* keys;
    u32 count;

    UnkTypeConversion_ovb0053** Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return &values[count];
    }
};
}  // namespace

static inline bool IsKindOf_ovb0053(const CType* t, const CType* base) {
    do {
        if (t == base)
            return true;
        t = t->m_parent;
    } while (t);
    return false;
}

static inline UnkConversionTable_ovb0053* ConversionsOf_ovb0053(const CType* t) {
    return reinterpret_cast<UnkConversionTable_ovb0053*>(const_cast<base::reflection::UntypedSmallDictionary*>(&t->m_conversions));
}

static inline u64 NameKey_ovb0053(const CType* t) {
    return t->sName.data->Key();
}

// Releases the type's attribute container (CType+0x80): the keys, the values, the arrays and the container.
__attribute__((always_inline)) void fn_710009cee0(CType* type) {
    auto* attrs = static_cast<UnkTypeAttrs_ovb0053*>(type->m_attributes);
    if (!attrs)
        return;
    if (attrs->keys) {
        for (u64 i = 0; i < attrs->count; ++i)
            attrs->keys[i].Reset();
    }
    if (attrs->values) {
        for (u64 i = 0; i < attrs->count; ++i)
            fn_71000a1c60(&attrs->values[i]);
    }
    attrs->count = 0;
    if (attrs->keys)
        attrs->allocator->vf_0x48(attrs->keys);
    if (attrs->values)
        attrs->allocator->vf_0x48(attrs->values);
    fn_7100083c00(attrs);
    type->m_attributes = nullptr;
}

void fn_710009cc04(CType* self) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a1b540[2];
    fn_710009cee0(self);
    base::reflection::UntypedSmallDictionary& links = self->m_conversions;
    links.count = 0;
    if (links.keys)
        links.allocator->vf_0x48(links.keys);
    if (links.data)
        links.allocator->vf_0x48(links.data);
    self->m_derivedTypes.Free();
    static_cast<CStrId&>(self->sName).Reset();
}

// The held value's typed pointer: its dynamic type (CType+0x68) and the value, stored inline when it
// fits in 8 bytes, else through the first word.
struct UnkReflHeldType_ovb0053 {
    u8 pad_0x0[0x18];
    u64 size;
    u8 pad_0x20[0x48];
    char* (*dynamic_type)(void* obj);
};

struct UnkReflHolder_ovb0053 {
    void* heap;
    UnkReflHeldType_ovb0053* type;
    u8 pad_0x10[0x4];
    s32 mode;
};

UnkReflTypedPtr fn_710009ee40(UnkReflHolder_ovb0053& holder) {
    UnkReflHolder_ovb0053* self = &holder;
    UnkReflTypedPtr r;
    UnkReflHeldType_ovb0053* type = self->type;
    if (!type) {
        r.type = nullptr;
        r.ptr = nullptr;
        return r;
    }
    void* obj = self->mode != 1 && type->size <= 8 ? static_cast<void*>(self) : self->heap;
    char* t = reinterpret_cast<char*>(type);
    if (obj)
        t = type->dynamic_type ? type->dynamic_type(obj) : nullptr;
    r.type = t;
    r.ptr = obj;
    return r;
}

// The value an attribute holds under `key`, or an empty value when the type has none.
UnkReflValue CType_GetAttributeValue(const CType* type, const UnkReflAttrKey& key) {
    UnkReflValue r;
    if (auto* attrs = static_cast<UnkAttrTable_ovb0053*>(type->m_attributes)) {
        UnkReflValue* it = attrs->Find(key);
        if (it != &attrs->values[attrs->count]) {
            r.field_0x0 = nullptr;
            r.type = nullptr;
            r.kind = it->kind;
            const CType* t = reinterpret_cast<const CType*>(it->type);
            const void* value = nullptr;
            if (t)
                value = it->field_0x14 == 1 || t->m_size > 8 ? it->field_0x0 : static_cast<const void*>(it);
            fn_71000a18d4(&r, t, value, false);
            return r;
        }
    }
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

// Assigns `src` (which must be of `type`) to `dst`: with the type's assignment (CType+0x48) when dst is
// of the same type, else through the conversion registered for dst's type on `type` or an ancestor.
bool CType_AssignOrConvertValue(const CType* type, UnkTypedRef* src, UnkTypedRef* dst) {
    if (src->type != type)
        return false;
    if (dst->type == type) {
        void* to = dst->ptr;
        void* from = src->ptr;
        type->m_copyAssign(to, from);
    } else {
        u64 key = NameKey_ovb0053(dst->type);
        const CType* t = type;
        UnkTypeConversion_ovb0053** p;
        for (;;) {
            UnkConversionTable_ovb0053* table = ConversionsOf_ovb0053(t);
            p = table->Find(key);
            if (p != &table->values[table->count])
                break;
            t = t->m_parent;
            if (!t)
                return false;
        }
        if (!*p)
            return false;
        void* from = src->ptr;
        void* to = dst->ptr;
        (*p)->convert(to, from);
    }
    return true;
}

// The object's dynamic type (CType+0x68), null when the reference has no type or the type no getter.
static inline const CType* DynamicTypeOf_ovb0053(const UnkTypedRef* ref) {
    const CType* type = ref->type;
    if (type && IsKindOf_ovb0053(type, type) && type->m_typeOf)
        return &type->m_typeOf(ref->ptr);
    return nullptr;
}

// Whether two values whose dynamic type is `type` are equal: by the type's comparison (CType+0x58), else by
// identity.
bool CType_AreValuesEqual(const CType* type, UnkTypedRef* a, UnkTypedRef* b) {
    const CType* ta = DynamicTypeOf_ovb0053(a);
    const CType* tb = DynamicTypeOf_ovb0053(b);
    if (ta == type && tb == type) {
        if (type->m_equals)
            return type->m_equals(a->ptr, b->ptr);
        return a->ptr == b->ptr;
    }
    return false;
}

// `src` (of `type`) as a value of type `to`: a reference when `type` is or derives from `to`, else a new
// value of `to` filled by the registered conversion; an empty value when there is none.
UnkReflValue fn_710009da40(const CType* type, const UnkTypedRef* src, const CType* to) {
    UnkReflValue r;
    if (type && IsKindOf_ovb0053(type, to)) {
        fn_71000a1978(&r, src, false, 2);
        return r;
    }
    u64 key = NameKey_ovb0053(to);
    const CType* t = type;
    UnkTypeConversion_ovb0053* conversion = nullptr;
    do {
        UnkConversionTable_ovb0053* table = ConversionsOf_ovb0053(t);
        UnkTypeConversion_ovb0053** p = table->Find(key);
        if (p != &table->values[table->count]) {
            conversion = *p;
            break;
        }
        t = t->m_parent;
    } while (t);
    if (conversion) {
        void* from = src->ptr;
        r.field_0x0 = nullptr;
        r.type = nullptr;
        r.kind = 2;
        r.field_0x14 = 1;
        fn_71000a2014(&r, to);
        void (*convert)(void*, void*) = conversion->convert;
        void* dst = nullptr;
        if (r.type)
            dst = r.field_0x14 == 1 || reinterpret_cast<const CType*>(r.type)->m_size > 8 ? r.field_0x0
                                                                                           : static_cast<void*>(&r);
        convert(dst, from);
        return r;
    }
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

// Adds the type's parent, if any, to `out`.
void CType_AppendParentType(const CType* self, base::global::CRntVector<const CType*>* out) {
    if (self->m_parent)
        out->PushBack(self->m_parent);
}

static inline bool Contains_ovb0053(const base::global::CRntVector<CType*>* v, const CType* t) {
    for (u64 i = 0; i < v->count; ++i) {
        if (v->data[i] == t)
            return true;
    }
    return false;
}

// Adds every type `type` refers to (its vtable slot 0x30), and recursively the types those refer to, to
// `out`, each once.
void fn_710009d1ec(CType* type, base::global::CRntVector<CType*>* out, bool flag) {
    base::global::CRntVector<CType*> refs(0xf);
    reinterpret_cast<UnkTypeVirtuals_ovb0053*>(type)->vf_0x30(&refs, flag);
    CType** begin = refs.data;
    CType** end = begin + refs.count;
    for (CType** it = begin; it != end; ++it) {
        CType* ref = *it;
        if (Contains_ovb0053(out, ref))
            continue;
        out->PushBack(ref);
        fn_710009d1ec(ref, out, flag);
    }
    refs.Free();
}

// The type's attribute container (CType+0x80), created on first use (fn_710009dd6c inlined).
static inline UnkReflAttrs* AttrsOf_ovb0053(CType* type) {
    auto* attrs = static_cast<UnkReflAttrsInit_ovb0053*>(type->m_attributes);
    if (!attrs) {
        attrs = static_cast<UnkReflAttrsInit_ovb0053*>(fn_71000839a0(0x38, 0x3e));
        attrs->values = nullptr;
        attrs->keys = nullptr;
        attrs->count = 0;
        attrs->capacity = 0;
        attrs->step = 10;
        attrs->flags = 0x3e;
        attrs->allocator = GetDefaultAllocator();
        attrs->field_0x28 = nullptr;
        attrs->field_0x30 = 0;
        type->m_attributes = attrs;
    }
    return reinterpret_cast<UnkReflAttrs*>(attrs);
}

// The type's own hash (vtable slot 0x38), computed once and kept at +0x24.
static inline s32 OwnHash_ovb0053(CType* type) {
    if (type->m_ownHash == -1)
        type->m_ownHash = reinterpret_cast<UnkTypeVirtuals_ovb0053*>(type)->vf_0x38();
    return type->m_ownHash;
}

// Stores the "AssetHash" attribute: the type's own hash combined with those of every type it refers to.
void CType_AddAssetHashAttribute(CType* type) {
    u32 hash = OwnHash_ovb0053(type);
    base::global::CRntVector<CType*> refs(0xf);
    fn_710009d1ec(type, &refs, true);
    CType** begin = refs.data;
    CType** end = begin + refs.count;
    for (CType** it = begin; it != end; ++it)
        hash ^= OwnHash_ovb0053(*it);
    UnkReflAttrs* attrs = AttrsOf_ovb0053(type);
    attrs->Add(fn_71000905e0(), hash);
    refs.Free();
}
