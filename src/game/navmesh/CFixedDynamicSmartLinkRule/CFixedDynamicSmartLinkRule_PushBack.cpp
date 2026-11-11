// In the navmesh::CFixedDynamicSmartLinkRule unit: appends an entry (0x50 bytes) to an array
// (base/UnkVector.h's layout), growing it to the next multiple of the step through fn_7100272474
// first and moving the entry in (its inner array is taken over and the source's emptied).
// Names and layouts are placeholders.
#include <new>

#include "game/navmesh/CFixedDynamicSmartLinkRule.h"
#include "types.h"

struct UnkSmartLinkEntry {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    void* data;  // 0x10
    u32 count;   // 0x18
    u32 capacity;
    u32 step;    // 0x20
    bool field_0x24;
    u32 flags;   // 0x28
    void* allocator;  // 0x30
    u8 field_0x38[0x15];

    UnkSmartLinkEntry(UnkSmartLinkEntry&& o)
        : field_0x0(o.field_0x0), field_0x4(o.field_0x4), field_0x8(o.field_0x8),
          field_0xc(o.field_0xc), data(o.data), count(o.count), capacity(o.capacity),
          step(o.step), field_0x24(o.field_0x24), flags(o.flags), allocator(o.allocator) {
        o.data = nullptr;
        o.count = 0;
        o.capacity = 0;
        __builtin_memcpy(field_0x38, o.field_0x38, sizeof(field_0x38));
    }
};

struct UnkSmartLinkEntryArray {
    UnkSmartLinkEntry* data;
    u32 count;
    u32 capacity;
    u32 step;
};

void fn_7100273ad4(UnkSmartLinkEntryArray* self, UnkSmartLinkEntry* value) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    if (capacity <= n && self->step != 0)
        capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_7100272474(self, capacity);
    new (&self->data[self->count]) UnkSmartLinkEntry(static_cast<UnkSmartLinkEntry&&>(*value));
    ++self->count;
}
