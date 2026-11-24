// base::reflection's unit (tools/tu_map.py unit 79): copy-constructs an array of 0x38-byte parameter
// attribute containers: CRntVector's copy constructor written out (take the source's settings and
// allocator, reserve its capacity, then append a copy of each container). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CBlackboard.h"
#include "types.h"

// An attribute container's destructor (0x38 bytes; base/UnkCloneHelpers.h).
void fn_710009abf8(void* attrs);
void fn_7100095d68(UnkPropertyArrays* self, const UnkPropertyArrays* other);

// A parameter attribute container: a keyed array of values with its growth settings (an
// UnkPropertyArrays), and two more words.
struct UnkReflParamAttrs_l09a850 {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;
    u64 field_0x28;
    u32 field_0x30;

    UnkReflParamAttrs_l09a850(const UnkReflParamAttrs_l09a850& other) {
        fn_7100095d68(reinterpret_cast<UnkPropertyArrays*>(this), reinterpret_cast<const UnkPropertyArrays*>(&other));
        u64 w28 = other.field_0x28;
        u32 w30 = other.field_0x30;
        field_0x30 = w30;
        field_0x28 = w28;
    }
    UnkReflParamAttrs_l09a850(UnkReflParamAttrs_l09a850&& other)
        : values(other.values), keys(other.keys), count(other.count), capacity(other.capacity), step(other.step),
          flags(other.flags), allocator(other.allocator) {
        u64 w28 = other.field_0x28;
        u32 w30 = other.field_0x30;
        other.count = 0;
        other.values = nullptr;
        other.keys = nullptr;
        field_0x30 = w30;
        field_0x28 = w28;
    }
    ~UnkReflParamAttrs_l09a850() { fn_710009abf8(this); }
};
static_assert(sizeof(UnkReflParamAttrs_l09a850) == 0x38, "layout");

using UnkReflParamAttrsVector_l09a850 = base::global::CRntVector<UnkReflParamAttrs_l09a850>;

void fn_710009a850(UnkReflParamAttrsVector_l09a850* self, const UnkReflParamAttrsVector_l09a850* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkReflParamAttrs_l09a850 *it = other->data, *end = it + other->count; it != end; ++it) {
        // Append, growing to the next multiple of the step when full.
        u32 n = self->count;
        u32 cap = self->capacity;
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        if (cap < new_capacity) {
            self->Reserve(new_capacity);
            n = self->count;
        }
        new (&self->data[n]) UnkReflParamAttrs_l09a850(*it);
        ++self->count;
    }
}
