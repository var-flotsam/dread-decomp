// From unit 79 of tools/tu_map.py (0x710009a5b4-0x710009bed8, base::reflection): the names of a
// reflected enumeration's values. Each value is named by the string-id attribute that its own
// attribute set (one 0x38-byte set per value, at +0x88) holds under the key at +0x1958 of the id
// table (the empty id without one). Names and layouts are placeholders.
// Holds fn_710009b050.
#include <new>

#include "base/UnkAtExit.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

// The size of a value of the type (CType+0x18).
static inline u64 TypeSize(const CType* type) {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(type) + 0x18);
}

// A reflected value as an attribute set stores it: its type, and the value inline when it fits in
// 8 bytes (else through field_0x0).
struct UnkAttrValue {
    void* field_0x0;
    const CType* type;
    u32 kind;
    u32 field_0x14;

    UnkAttrValue() : field_0x0(nullptr), type(nullptr), kind(2), field_0x14(1) {}
    UnkAttrValue(const UnkAttrValue& other) {
        field_0x0 = nullptr;
        type = nullptr;
        kind = other.kind;
        fn_71000a18d4(Raw(), other.type, other.type ? other.Value() : nullptr, false);
    }
    ~UnkAttrValue() { fn_71000a1c60(Raw()); }

    UnkReflValue* Raw() { return reinterpret_cast<UnkReflValue*>(this); }
    const void* Value() const { return (field_0x14 == 1 || TypeSize(type) > 8) ? field_0x0 : this; }
};
static_assert(sizeof(UnkAttrValue) == 0x18, "UnkAttrValue layout");

// An attribute set: the values and their keys (the key names' string data), `count` of each.
struct UnkAttrSet {
    UnkAttrValue* values;
    const void** keys;
    u32 count;
    u8 pad_0x14[0x24];

    const UnkAttrValue* Find(const CStrId& key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key.data)
                return &values[i];
        return values + count;
    }
};
static_assert(sizeof(UnkAttrSet) == 0x38, "UnkAttrSet placeholder layout");

// The enumeration: its values, and one attribute set per value.
struct UnkEnumType {
    u8 pad_0x0[0x8];
    void** values;
    u32 count;
    u8 pad_0x14[0x74];
    UnkAttrSet* attrs;
    u32 attr_count;
};
static_assert(__builtin_offsetof(UnkEnumType, attrs) == 0x88, "UnkEnumType placeholder layout");

// The id table (fn_7100031894 builds it; src/game/CGameManager.cpp has the same definition).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x17e0];
    u8 field_0x17e0[8];
    u8 pad_0x17e8[0x2578 - 0x17e8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// A value's name: copied from a string id, moved by taking the other's string (leaving it the
// empty one), released like CStrId.
struct UnkEnumNameId {
    base::global::UnkStringData* data;

    UnkEnumNameId(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkEnumNameId(UnkEnumNameId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkEnumNameId() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

// A value and its name. The value is held in a union of two types (copied with no type-based
// alias info): the original reloads the array's count and the string pool after storing it.
struct UnkEnumName {
    UnkEnumNameId name;
    union {
        void* value;
        u64 bits;
    };
};
static_assert(sizeof(UnkEnumName) == 0x10, "UnkEnumName layout");

// Reserves room for n names (out of line).
void fn_710009b46c(base::UnkVector<UnkEnumName>* v, u32 n);

static inline void PushBack(base::UnkVector<UnkEnumName>* v, UnkEnumName&& e) {
    u32 n = v->count;
    u32 cap = v->capacity;
    if (cap <= n && v->step != 0)
        cap = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)cap)
        for (;;) {
        }
    fn_710009b46c(v, cap);
    new (&v->data[v->count]) UnkEnumName(static_cast<UnkEnumName&&>(e));
    ++v->count;
}

// A string id holding the empty string.
struct UnkDefaultStrId {
    union {
        CStrId id;
    };

    UnkDefaultStrId() {
        id.data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        id.data = &g_unk_7101d47108->empty;
    }
    ~UnkDefaultStrId() { id.~CStrId(); }
};

// The names, returned in memory (the copy is declared for the return only; it is elided).
struct UnkEnumNames : base::UnkVector<UnkEnumName> {
    explicit UnkEnumNames(u32 flags) : base::UnkVector<UnkEnumName>(flags) {}
    UnkEnumNames(const UnkEnumNames& other);
};

// NON_MATCHING(minor): one equality compare has its operands swapped (cmp x23, x8 for cmp x8, x23:
// the copied name against the pool's empty string after the attribute is read).
UnkEnumNames fn_710009b050(const UnkEnumType* type) {
    UnkEnumNames names(0x59);
    fn_710009b46c(&names, type->count);
    for (u64 i = 0; i < type->count; ++i) {
        UnkDefaultStrId name;
        if (i < type->attr_count) {
            const UnkAttrSet& attrs = type->attrs[i];
            const CStrId& key = *reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0x0 + 0x1958);
            const UnkAttrValue* it = attrs.Find(key);
            UnkAttrValue value = it != attrs.values + attrs.count ? *it : UnkAttrValue();
            if (value.type && fn_710009d50c(value.type, fn_7100009658()))
                name.id = *static_cast<const CStrId*>(value.Value());
        }
        PushBack(&names, UnkEnumName{name.id, type->values[i]});
    }
    return names;
}
