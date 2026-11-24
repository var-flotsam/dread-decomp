// Unit 94 (base::serialization::CTypedRefWriter's unit): ends the innermost context of the writer's
// stack, first inserting its value into the enclosing context's collection at that context's key.
// Names and layouts are placeholders.
#include "base/reflection.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkHeldType_f29a0.h"
#include "game/UnkTypedRefEntry_f29a0.h"
#include "types.h"

using base::reflection::CCollectionType;
using base::reflection::CType;
using base::reflection::UnkReflValue;

static_assert(sizeof(UnkTypedRefEntry_f29a0) == 0xd8, "entry layout");

struct UnkTypedRefWriter_l0f21bc {
    u32 field_0x0;
    u32 count;
    alignas(8) u8 storage[sizeof(UnkTypedRefEntry_f29a0)];

    UnkTypedRefEntry_f29a0* entries() { return reinterpret_cast<UnkTypedRefEntry_f29a0*>(storage); }
};

// A typed reference to a value (a type and where the value is).
struct UnkReflValueArg {
    const CType* type;
    void* ptr;
};

// One context of the writer: the value being written and, inside a collection, its key.
struct UnkWriterContext_f21bc {
    u8 pad_0x0[0x8];
    UnkReflValue value;  // 0x8
    u8 pad_0x20[0x78];
    bool field_0x98;
    u8 pad_0x99[0x7];
    UnkReflValue key;  // 0xa0
    bool field_0xb8;
};
static_assert(__builtin_offsetof(UnkWriterContext_f21bc, field_0xb8) == 0xb8, "context layout");
static_assert(__builtin_offsetof(CCollectionType, m_key) == 0xe8, "CCollectionType layout");
static_assert(__builtin_offsetof(CCollectionType, m_add_copy) == 0x118, "CCollectionType layout");

using UnkCollectionInsertFn_f21bc = void (*)(void* collection, void* key, void* value);

// Sets the number of contexts: the ones dropped have their held storage destroyed, the ones added
// start empty.
static inline void SetCount(UnkTypedRefWriter_l0f21bc* self, u32 count) {
    if (count < self->count) {
        for (u64 i = count; i < self->count; ++i) {
            UnkTypedRefEntry_f29a0& entry = self->entries()[i];
            if (entry.storage_type) {
                entry.storage_type->destroy(entry.storage);
                entry.storage_type = nullptr;
            }
        }
    } else {
        for (u64 i = self->count; i < count; ++i) {
            self->entries()[i].context = nullptr;
            self->entries()[i].storage_type = nullptr;
        }
    }
    self->count = count;
}

// The context at `index`, growing the stack to reach it.
static inline UnkWriterContext_f21bc* ContextAt(UnkTypedRefWriter_l0f21bc* self, u32 index) {
    if (self->count < index + 1)
        SetCount(self, index + 1);
    return static_cast<UnkWriterContext_f21bc*>(self->entries()[index].context);
}

// Where a reflected value's data is: inline when it fits in 8 bytes, else through field_0x0.
static inline void* ValueAddress(UnkReflValue& value) {
    if (!value.type)
        return nullptr;
    if (value.field_0x14 == 1 || reinterpret_cast<const CType*>(value.type)->m_size > 8)
        return value.field_0x0;
    return &value;
}

bool fn_71000f21bc(UnkTypedRefWriter_l0f21bc* self) {
    UnkWriterContext_f21bc* parent = ContextAt(self, self->count - 2);
    if (!parent->field_0xb8 && !parent->field_0x98) {
        const CType* type = reinterpret_cast<const CType*>(parent->value.type);
        void* collection = ValueAddress(parent->value);
        auto* top = static_cast<UnkWriterContext_f21bc*>(self->entries()[self->count - 1].context);
        UnkReflValueArg key;
        fn_71000a1710(&key, &parent->key);
        const CType* value_type = reinterpret_cast<const CType*>(top->value.type);
        void* value = ValueAddress(top->value);
        const CCollectionType* coll = nullptr;
        if (type && fn_710009d50c(reinterpret_cast<const base::reflection::UnkReflObject*>(type)->vf_0x0(), fn_71000986d0()))
            coll = static_cast<const CCollectionType*>(type);
        if (coll->m_add_copy && type == coll && coll->m_key == key.type && coll->m_element == value_type)
            reinterpret_cast<UnkCollectionInsertFn_f21bc>(coll->m_add_move)(collection, key.ptr, value);
    }
    SetCount(self, self->count - 1);
    return true;
}
