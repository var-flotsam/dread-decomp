// Unit 365: appends a copy of a named record (id, small fields, sub-object, u16 array) to a growable
// array after reserving room. Names and layouts are placeholders.
#include <new>
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

// A copy of a growable array of 16-bit values: same settings, own storage.
struct UnkU16Array_b0228 {
    u16* data;                      // 0x0
    u32 count;                      // 0x8
    u32 capacity;                   // 0xc
    u32 step;                       // 0x10
    bool field_0x14;                // 0x14
    u32 flags;                      // 0x18
    base::UnkAllocator* allocator;  // 0x20

    UnkU16Array_b0228() = default;
    UnkU16Array_b0228(const UnkU16Array_b0228& o) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = o.step;
        field_0x14 = o.field_0x14;
        flags = o.flags;
        allocator = o.allocator;
        u32 cap = o.capacity;
        u16* p = nullptr;
        if (cap) {
            p = static_cast<u16*>(allocator->vf_0x40((u64)cap * 2, 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * 2);
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = cap;
        }
        __builtin_memcpy(p, o.data, (u64)o.count * 2);
        count = o.count;
    }
};

// The sub-object copied by fn_71002a9df0.
struct UnkSubObject2_b0228 {
    alignas(8) u8 data[0x28];
    UnkSubObject2_b0228() = default;
    UnkSubObject2_b0228(const UnkSubObject2_b0228&) asm("fn_71002a9df0");
};

struct UnkNamedRecord_b0228 {
    base::global::CStrId name;  // 0x0
    struct __attribute__((packed)) Small {
        u16 field_0x8;
        u8 field_0xa;
    } small;                  // 0x8
    UnkSubObject2_b0228 sub;  // 0x10
    UnkU16Array_b0228 list;   // 0x38

    __attribute__((always_inline)) UnkNamedRecord_b0228(const UnkNamedRecord_b0228& o)
        : name(o.name), small(o.small), sub(o.sub), list(o.list) {}
};
static_assert(sizeof(UnkNamedRecord_b0228) == 0x60, "UnkNamedRecord_b0228 layout");

struct UnkNamedRecordArray_b0228 {
    UnkNamedRecord_b0228* data;  // 0x0
    u32 count;                   // 0x8
    u32 capacity;                // 0xc
    u32 step;                    // 0x10
};

void fn_71002a9c24(UnkNamedRecordArray_b0228* self, u32 capacity);

void fn_71002a9aa0(UnkNamedRecordArray_b0228* self, const UnkNamedRecord_b0228* value) {
    u32 n = self->count;
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0)
        cap = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)cap)
        for (;;) {
        }
    fn_71002a9c24(self, cap);
    new (&self->data[self->count]) UnkNamedRecord_b0228(*value);
    self->count = self->count + 1;
}
