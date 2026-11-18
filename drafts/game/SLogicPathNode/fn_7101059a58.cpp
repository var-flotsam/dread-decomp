// SLogicPathNode unit: erases the 0x20-byte polymorphic element at an index (destroyed through vtable
// slot 1) and moves each later element down one, destroying the one moved from.
// Names and layouts are placeholders.
// build-flags: no-strict-aliasing
#include <new>

#include "game/SLogicPathNode.h"
#include "types.h"

struct UnkPathPoint_l059a58 {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    void* field_0x10;
    u32 field_0x18;
    u8 pad_0x1c[0x4];

    UnkPathPoint_l059a58(UnkPathPoint_l059a58&& other)
        : vtable(reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10)),
          field_0x8(other.field_0x8),
          field_0xc(other.field_0xc),
          field_0x10(other.field_0x10),
          field_0x18(other.field_0x18) {}
    void Destroy() { reinterpret_cast<void (*)(UnkPathPoint_l059a58*)>(vtable[1])(this); }
};

struct UnkPathPointList_l059a58 {
    u8 pad_0x0[0x8];
    UnkPathPoint_l059a58* data;  // 0x8
    u32 count;                   // 0x10
};

void fn_7101059a58(UnkPathPointList_l059a58* self, u32 index) {
    if (index < self->count) {
        self->data[index].Destroy();
        for (u64 i = index; i < self->count - 1; ++i) {
            new (&self->data[i]) UnkPathPoint_l059a58(static_cast<UnkPathPoint_l059a58&&>(self->data[i + 1]));
            self->data[i + 1].Destroy();
        }
        self->count = self->count - 1;
    }
}
