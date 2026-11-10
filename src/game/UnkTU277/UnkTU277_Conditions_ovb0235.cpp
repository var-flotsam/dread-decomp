// A unit with no known name (tools/tu_map.py unit 277, 0x71001f9d74-0x71001fa6d0): arrays of 0x18-byte
// conditions (a key and a kind, base/UnkVector.h's layout and the 0x10 and 0x18 members of the list
// that holds them). fn_71001f9d74 removes the first condition equal to a key, shifting the later ones
// down; fn_71001fa4e4 copy-constructs the array (flags, step and allocator taken over, the capacity
// reserved, each condition appended). Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "types.h"

// A condition. The destructor makes it non-trivially copyable, so the array's growth moves its
// elements one by one (a block copy each).
struct UnkCondition_b0235 {
    u64 key;       // 0x0
    u64 field_0x8;
    u32 kind;      // 0x10
    u32 field_0x14;

    ~UnkCondition_b0235() {}
};

struct UnkConditionList_b0235 {
    u8 pad_0x0[0x10];
    UnkCondition_b0235* data;  // 0x10
    u32 count;                 // 0x18
};

// Removes the first condition equal to `key`, shifting the later ones down.
void fn_71001f9d74(UnkConditionList_b0235* self, const UnkCondition_b0235* key) {
    u32 n = self->count;
    for (u64 i = 0; i != n; ++i) {
        if (self->data[i].kind == key->kind && self->data[i].key == key->key) {
            u32 last = n - 1;
            if (last > (u32)i) {
                for (;;) {
                    self->data[i] = self->data[i + 1];
                    ++i;
                    last = self->count - 1;
                    if (i >= last)
                        break;
                }
            }
            self->count = last;
            return;
        }
    }
}

struct UnkConditionVector_b0235;

// Copy-constructs the array of conditions.
__attribute__((flatten)) void fn_71001fa4e4(UnkConditionVector_b0235* self_, const UnkConditionVector_b0235* other_) {
    auto* self = reinterpret_cast<base::UnkVector<UnkCondition_b0235>*>(self_);
    auto* other = reinterpret_cast<const base::UnkVector<UnkCondition_b0235>*>(other_);
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    if (other->count != 0) {
        const UnkCondition_b0235* end = other->data + other->count;
        for (const UnkCondition_b0235* p = other->data; p != end; ++p)
            self->PushBackCopy(*p);
    }
}
