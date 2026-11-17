// Unit 70: adding every attribute of another element's attribute arrays to these, each replacing
// a value already held under its key. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

UnkReflValue* fn_7100093a50(UnkPropertyArrays* self, UnkReflValue* slot);
// One attribute as an iterator over the attribute arrays yields it: the key and value pointed to
// (kind 2), or a key and value of its own (kind 1), or nothing (kind 0).
struct UnkReflAttrEntry {
    struct Owned {
        CStrId key;
        UnkReflValue value;
    };
    struct Ref {
        CStrId* key;
        UnkReflValue* value;
    };
    alignas(8) u8 storage[0x20];
    u32 kind;

    UnkReflAttrEntry(CStrId* key, UnkReflValue* value) : kind(2) {
        Refs().key = key;
        Refs().value = value;
    }
    ~UnkReflAttrEntry() {
        switch (kind) {
        case 1:
            fn_71000a1c60(&Own().value);
            Own().key.~CStrId();
            break;
        case 2:
            break;
        default:
            return;
        }
        kind = 0;
    }

    Owned& Own() { return *reinterpret_cast<Owned*>(storage); }
    Ref& Refs() { return *reinterpret_cast<Ref*>(storage); }
    CStrId* Key() { return kind == 2 ? Refs().key : &Own().key; }
    UnkReflValue* Value() { return kind == 2 ? Refs().value : &Own().value; }
    void operator++() {
        ++Refs().key;
        ++Refs().value;
    }
    bool operator!=(UnkReflAttrEntry& other) { return Refs().key != other.Refs().key; }
};

// The value held under the entry's key, or the end of the values.
inline UnkReflValue* FindAttrValue(UnkPropertyArrays* self, const CStrId* key) {
    for (u32 i = 0; i < self->count; ++i)
        if (self->ids[i].data == key->data)
            return &static_cast<UnkReflValue*>(self->values)[i];
    return &static_cast<UnkReflValue*>(self->values)[self->count];
}

// Where a value's data is: inline when it fits in 8 bytes, else through field_0x0.
inline const void* AttrValueData(const UnkReflValue* v, const CType* type) {
    if (!type)
        return nullptr;
    if (v->field_0x14 == 1 || type->m_size >= 9)
        return v->field_0x0;
    return v;
}

void fn_71000937a8(UnkPropertyArrays* self, const UnkPropertyArrays* other) {
    u32 count = other->count;
    auto* values = static_cast<UnkReflValue*>(other->values);
    UnkReflAttrEntry end(&other->ids[count], &values[count]);
    for (UnkReflAttrEntry it(other->ids, values); it != end; ++it) {
        UnkReflValue* slot = FindAttrValue(self, it.Key());
        if (slot != &static_cast<UnkReflValue*>(self->values)[self->count])
            fn_7100093a50(self, slot);
        UnkReflValue* value = it.Value();
        CStrId* key = it.Key();
        if (self->count == self->capacity)
            fn_7100093dd0(self, self->step + self->count);
        new (&self->ids[self->count]) CStrId(*key);
        UnkReflValue* copy = &static_cast<UnkReflValue*>(self->values)[self->count];
        copy->field_0x0 = nullptr;
        copy->type = nullptr;
        copy->kind = value->kind;
        const CType* type = reinterpret_cast<const CType*>(value->type);
        fn_71000a18d4(copy, type, AttrValueData(value, type), 0);
        ++self->count;
    }
}