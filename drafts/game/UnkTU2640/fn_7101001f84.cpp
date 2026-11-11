#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkElem78 {
    u8 unk_0x0[0x78];
};

template <class Elem>
struct UnkElemArray {
    Elem* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710100147c(UnkElemArray<UnkElem78>* self, u32 capacity);

struct UnkRecord78_b0212 {
    u32 field_0x0;
    void* data;     // 0x8
    u32 size;       // 0x10
    u32 capacity;   // 0x14
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    u64 field_0x28;
    u8 tail[0x1d];  // 0x30
    void* data2;    // 0x50
    u32 size2;      // 0x58
    u32 capacity2;  // 0x5c
    u32 field_0x60;
    bool field_0x64;
    u32 field_0x68;
    u64 field_0x70;

    UnkRecord78_b0212(UnkRecord78_b0212&& o) {
        UnkRecord78_b0212* other = &o;
        field_0x0 = other->field_0x0;
        data = other->data;
        size = other->size;
        capacity = other->capacity;
        field_0x18 = other->field_0x18;
        field_0x1c = other->field_0x1c;
        field_0x20 = other->field_0x20;
        field_0x28 = other->field_0x28;
        other->data = nullptr;
        other->size = 0;
        other->capacity = 0;
        __builtin_memcpy(tail, other->tail, sizeof(tail));
        data2 = other->data2;
        size2 = other->size2;
        capacity2 = other->capacity2;
        field_0x60 = other->field_0x60;
        field_0x64 = other->field_0x64;
        field_0x68 = other->field_0x68;
        field_0x70 = other->field_0x70;
        other->data2 = nullptr;
        other->size2 = 0;
        other->capacity2 = 0;
    }
};

void fn_7101001f84(UnkElemArray<UnkRecord78_b0212>* self, UnkRecord78_b0212* value) {
    u32 n = self->count;
    u32 old_capacity = self->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_710100147c(reinterpret_cast<UnkElemArray<UnkElem78>*>(self), new_capacity);
    new (&self->data[self->count]) UnkRecord78_b0212(static_cast<UnkRecord78_b0212&&>(*value));
    ++self->count;
}
