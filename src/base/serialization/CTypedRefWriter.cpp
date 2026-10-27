// base::serialization::CTypedRefWriter's unit (tools/tu_map.py unit 94; the class and its contexts
// SBaseContext, SObjectContext and SCollectionContext are named by the unit's static initialiser,
// src/game_s1/fn_71000f3150.cpp). The writer keeps a stack of contexts; fn_71000f0648 resolves the
// value of the innermost one. Which context type is which, and the names and layouts below, are
// placeholders inferred from the code.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;

// The reflected value's copy-assignment (the declaration is src/game_s1/fn_71000d6310.cpp's, over a
// record of UnkReflValue's size).
struct UnkAssignedRecord_a1ba4 {
    u64 unk_0x0[0x18 / 8];
};
void fn_71000a1ba4(UnkAssignedRecord_a1ba4* self, const UnkAssignedRecord_a1ba4* other);

// A reflected value with its copy-assignment (fn_71000a1ba4) and destructor (fn_71000a1c60).
struct UnkOwnedReflValue : UnkReflValue {
    UnkOwnedReflValue& operator=(const UnkOwnedReflValue& other) {
        fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(this),
                      reinterpret_cast<const UnkAssignedRecord_a1ba4*>(&other));
        return *this;
    }
    ~UnkOwnedReflValue() { fn_71000a1c60(this); }

    // Rebuilds the value as a reference to `object` of type `type`.
    void SetRef(const CType* type, void* object) {
        fn_71000a1c60(this);
        fn_71000a18d4(this, type, object, 1);
    }
};

// The object a pointer value points to, with its dynamic type.
UnkReflTypedPtr fn_710009c460(const UnkReflTypedPtr& pointer);

// SBaseContext's class descriptor (src/game_s1/fn_71000f3150.cpp builds it).
extern UnkReflType g_unk_7101d67d80;

// The address of a reflected value's object: inline when it fits in 8 bytes, else through
// field_0x0; null for an empty value.
static inline void* ValueAddress(UnkReflValue* value) {
    if (!value->type)
        return nullptr;
    return value->field_0x14 == 1 || *reinterpret_cast<const u64*>(value->type + 0x18) > 8 ? value->field_0x0
                                                                                           : value;
}
static inline UnkReflTypedPtr TypedAddress(UnkReflValue* value) {
    UnkReflTypedPtr r;
    r.type = const_cast<char*>(value->type);
    r.ptr = ValueAddress(value);
    return r;
}

// The key a context reads its value under.
struct UnkTypedRefKey {
    u64 field_0x0;
    u64 field_0x8;

    UnkTypedRefKey(const u64& a, const u32& b) : field_0x0(a), field_0x8(b) {}
};

// What reads a context's value: from the source object and the key.
struct UnkTypedRefReader {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkOwnedReflValue vf_0x10(const UnkReflTypedPtr& source, const UnkTypedRefKey& key, u32 flags);
};

struct UnkTypedRefContext {
    void* vtable;
    UnkOwnedReflValue value;  // +0x8
    u8 pad_0x20[0x8];
    UnkReflValue source;  // +0x28
    UnkTypedRefReader* reader;
    u64 field_0x48;
    u32 field_0x50;
};

// A stack entry: the context in use and storage for one held in place, with the type of what the
// storage holds.
struct UnkTypedRefEntry {
    UnkTypedRefContext* context;
    u8 storage[0xc8];
    const CType* storage_type;
};
static_assert(sizeof(UnkTypedRefEntry) == 0xd8, "UnkTypedRefEntry placeholder layout");

// The context stack: a count and raw storage for the entries (the capacity is unknown).
struct UnkTypedRefWriter {
    u32 field_0x0;
    u32 count;
    alignas(8) u8 storage[sizeof(UnkTypedRefEntry)];

    UnkTypedRefEntry* entries() { return reinterpret_cast<UnkTypedRefEntry*>(storage); }
};

// Reads the innermost context's value through its reader. A pointer value is also handed to the
// enclosing context (the one held in place, when it is an SBaseContext), and the innermost value
// becomes a reference to the object it points to.
void fn_71000f0648(UnkTypedRefWriter* self) {
    UnkTypedRefContext* context = self->entries()[self->count - 1].context;
    context->value = context->reader->vf_0x10(TypedAddress(&context->source),
                                              UnkTypedRefKey(context->field_0x48, context->field_0x50), 0);
    if (!context->value.type)
        return;
    if (!fn_710009d50c(reinterpret_cast<const UnkReflObject*>(context->value.type)->vf_0x0(), fn_710009bedc()))
        return;
    UnkTypedRefContext* parent;
    if (self->count < 2) {
        parent = nullptr;
    } else {
        UnkTypedRefEntry* entry = &self->entries()[self->count - 2];
        parent = reinterpret_cast<UnkTypedRefContext*>(entry->storage);
        if (!fn_710009d50c(entry->storage_type, reinterpret_cast<const CType*>(&g_unk_7101d67d80)))
            parent = nullptr;
    }
    parent->value = context->value;
    UnkReflTypedPtr target = fn_710009c460(TypedAddress(&parent->value));
    context->value.SetRef(reinterpret_cast<const CType*>(target.type), target.ptr);
}
