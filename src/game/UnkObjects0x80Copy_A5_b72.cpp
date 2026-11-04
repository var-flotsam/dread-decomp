// Unit 1629 (tools/tu_map.py, no class hint): fn_7100ad20a8, a copy of fn_7100563d8c (CEmmyAIComponent_DefCopy_A3_b69):
// the copy constructor of an array of 0x80-byte objects (moved by fn_7100acf988, copied by fn_7100acfb14,
// destroyed by fn_7100ad04c4): the other's step, flags and allocator, its capacity reserved, then each object
// appended. Names are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_7100acf988(void*, void*);
void fn_7100acfb14(void*, const void*);
void fn_7100ad04c4(void*);

struct UnkObject0x80_A5_b72 {
    u64 unk_0x0[0x10];

    __attribute__((always_inline)) UnkObject0x80_A5_b72(UnkObject0x80_A5_b72&& other) { fn_7100acf988(this, &other); }
    __attribute__((always_inline)) UnkObject0x80_A5_b72(const UnkObject0x80_A5_b72& other) {
        fn_7100acfb14(this, &other);
    }
    __attribute__((always_inline)) ~UnkObject0x80_A5_b72() { fn_7100ad04c4(reinterpret_cast<char*>(this)); }
};
static_assert(sizeof(UnkObject0x80_A5_b72) == 0x80, "UnkObject0x80_A5_b72 layout");

using UnkObjects0x80_A5_b72 = base::UnkVector<UnkObject0x80_A5_b72>;

void fn_7100ad20a8(UnkObjects0x80_A5_b72* self, const UnkObjects0x80_A5_b72* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkObject0x80_A5_b72 *it = other->data, *end = other->data + other->count; it != end; ++it)
        self->PushBackCopy(*it);
}
