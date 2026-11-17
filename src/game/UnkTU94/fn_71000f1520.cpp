// build-flags: no-strict-aliasing
// Unit 94 (base::serialization::CTypedRefWriter's unit): ends the innermost context of the writer's
// stack, first writing its value into the enclosing object's field. Names and layouts are
// placeholders.
#include "base/reflection.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU85.h"
#include "game/UnkTU94.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

// The functions of a held type (only the destroy slot is named).
struct UnkHeldType_f29a0 {
    u8 pad_0x0[0x40];
    void (*destroy)(void* storage);
};

struct UnkTypedRefEntry_f29a0 {
    void* context;
    u8 storage[0xc8];
    UnkHeldType_f29a0* storage_type;  // 0xd0
};
static_assert(sizeof(UnkTypedRefEntry_f29a0) == 0xd8, "entry layout");

struct UnkTypedRefWriter {
    u32 field_0x0;
    u32 count;
    alignas(8) u8 storage[sizeof(UnkTypedRefEntry_f29a0)];

    UnkTypedRefEntry_f29a0* entries() { return reinterpret_cast<UnkTypedRefEntry_f29a0*>(storage); }
};

// An object and its type, as the walk passes it on.
struct UnkReflFieldObject {
    char* type;
    void* ptr;
};

// A typed reference to a value (a type and where the value is).
struct UnkReflValueArg {
    const CType* type;
    void* ptr;
};

// The field being written (only the slot this reads is named).
struct UnkWriterField_f1520 {
    u8 pad_0x0[0x28];
    void* field_0x28;
};

struct UnkWriterSource_f1520 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

// What every context of the writer has: the value being written.
struct UnkWriterContextBase_f1520 {
    virtual void vf_0x0();
    UnkReflValue value;  // 0x8
};

// A context writing an object's fields.
struct UnkWriterContext_f1520 : UnkWriterContextBase_f1520 {
    u8 pad_0x20[0x20];
    UnkWriterSource_f1520* source;  // 0x40
    u8 pad_0x48[0x8];
    u32 field_0x50;
    u8 pad_0x54[0x2c];
    void* field_0x80;
};
static_assert(__builtin_offsetof(UnkWriterContext_f1520, field_0x80) == 0x80, "context layout");

// Sets the number of contexts: the ones dropped have their held storage destroyed, the ones added
// start empty.
inline void SetCount_f1520(UnkTypedRefWriter* self, u32 count) {
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
inline UnkWriterContext_f1520* ContextAt_f1520(UnkTypedRefWriter* self, u32 index) {
    if (self->count < index + 1)
        SetCount_f1520(self, index + 1);
    return static_cast<UnkWriterContext_f1520*>(self->entries()[index].context);
}

// Where a reflected value's data is: inline when it fits in 8 bytes, else through field_0x0.
static inline void* ValueAddress(UnkReflValue& value) {
    if (!value.type)
        return nullptr;
    if (value.field_0x14 == 1 || reinterpret_cast<const CType*>(value.type)->m_size > 8)
        return value.field_0x0;
    return &value;
}

bool fn_71000f1520(UnkTypedRefWriter* self, UnkWriterField_f1520* field) {
    UnkWriterContext_f1520* parent = ContextAt_f1520(self, self->count - 2);
    if (parent->value.type || !parent->source) {
        if (field->field_0x28) {
            auto* top = static_cast<UnkWriterContext_f1520*>(self->entries()[self->count - 1].context);
            UnkReflFieldObject object;
            object.type = const_cast<char*>(parent->value.type);
            object.ptr = ValueAddress(parent->value);
            UnkReflValueArg value;
            fn_71000a1710(&value, &top->value);
            fn_71000e7a80(reinterpret_cast<UnkReflField*>(field), &object, &value);
        }
        SetCount_f1520(self, self->count - 1);
    } else {
        SetCount_f1520(self, self->count - 1);
        if (parent->source->field_0x10 == parent->field_0x50)
            fn_71000f0648(self);
    }
    parent->field_0x80 = nullptr;
    return true;
}
