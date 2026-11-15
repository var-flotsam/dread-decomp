// Unit 108: move assignment of a reflected iterator value (index and a 0x48-byte element) whose
// element moves its buffer pair when it is a different object. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "types.h"

struct UnkBufferElement108 {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[0x3];
    u32 field_0x18;
    u8 pad_0x1c[0x4];
    u64 field_0x20;
    u32 field_0x28;
    u8 field_0x2c;
    u8 field_0x2d;
    u8 pad_0x2e[0x12];
    u32 field_0x40;
    u8 pad_0x44[0x4];

    UnkBufferElement108& operator=(UnkBufferElement108&& other) {
        field_0x28 = other.field_0x28;
        field_0x2c = other.field_0x2c;
        field_0x2d = 1;
        if (this != &other) {
            field_0x8 = 0;
            field_0x20 = other.field_0x20;
            field_0x18 = other.field_0x18;
            field_0x10 = other.field_0x10;
            field_0x14 = other.field_0x14;
            field_0x8 = other.field_0x8;
            field_0xc = other.field_0xc;
            field_0x0 = other.field_0x0;
            other.field_0x0 = 0;
            other.field_0x8 = 0;
            other.field_0xc = 0;
        }
        field_0x40 = other.field_0x40;
        return *this;
    }
};
static_assert(sizeof(UnkBufferElement108) == 0x48, "UnkBufferElement108 layout");

using UnkBufferIterValue108 = base::UnkIterValue<UnkBufferElement108, u32>;

static inline UnkBufferIterValue108* MoveAssign(UnkBufferIterValue108* self, UnkBufferIterValue108* other) {
    UnkBufferElement108* element;
    if (other->state == 1) {
        self->Index() = other->value.index;
        element = &other->value.element;
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        element = other->ref.element;
    } else {
        return self;
    }
    self->Element() = static_cast<UnkBufferElement108&&>(*element);
    return self;
}

void* fn_710011e920(void* dst, const void* src) {
    return MoveAssign(static_cast<UnkBufferIterValue108*>(dst),
                      static_cast<UnkBufferIterValue108*>(const_cast<void*>(src)));
}
