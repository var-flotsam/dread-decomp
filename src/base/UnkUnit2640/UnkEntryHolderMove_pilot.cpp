// Move-constructs an entry holder (kind at +0x58: 0 none, 1 an owned key/value pair, 2 references to a
// key and a value elsewhere): the new holder always owns its pair (kind 1), moving the value's array
// out of the source. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

// An array of the engine's growable-array layout (base/UnkVector.h); moves take the other's array.
struct UnkMovedArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkMovedArray(UnkMovedArray&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

struct UnkEntryValue {
    u32 field_0x0;
    UnkMovedArray field_0x8;
    u8 field_0x30[0x1d];

    UnkEntryValue(UnkEntryValue&& other)
        : field_0x0(other.field_0x0), field_0x8(static_cast<UnkMovedArray&&>(other.field_0x8)) {
        __builtin_memcpy(field_0x30, other.field_0x30, sizeof(field_0x30));
    }
};

struct UnkEntryPair {
    u32 key;
    UnkEntryValue value;

    UnkEntryPair(u32 k, UnkEntryValue&& v) : key(k), value(static_cast<UnkEntryValue&&>(v)) {}
};

struct UnkEntryRefPair {
    const u32* key;
    UnkEntryValue* value;
};

struct UnkEntryHolder {
    union {
        UnkEntryPair pair;
        UnkEntryRefPair ref;
    };
    s32 kind;

    UnkEntryHolder(UnkEntryHolder&& other);
};
static_assert(__builtin_offsetof(UnkEntryHolder, kind) == 0x58, "UnkEntryHolder layout");

void fn_7100ffc81c(UnkEntryHolder* self, UnkEntryHolder* other) {
    self->kind = 0;
    if (other->kind == 1) {
        self->kind = 1;
        new (&self->pair) UnkEntryPair(other->pair.key, static_cast<UnkEntryValue&&>(other->pair.value));
    } else if (other->kind == 2) {
        self->kind = 1;
        new (&self->pair) UnkEntryPair(*other->ref.key, static_cast<UnkEntryValue&&>(*other->ref.value));
    }

}
