// SLogicPathNode unit: erases the 0x20-byte polymorphic element at an index (destroyed through vtable
// slot 1) and moves each later element down one, destroying the one moved from.
// Names and layouts are placeholders.
#include "types.h"

extern char g_unk_7101cac8f0[];

struct UnkPathPoint {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    void* field_0x10;
    u32 field_0x18;
    u8 pad_0x1c[0x4];

    void Destroy() { reinterpret_cast<void (*)(UnkPathPoint*)>(vtable[1])(this); }
};

struct UnkPathPointList {
    u8 pad_0x0[0x8];
    UnkPathPoint* data;  // 0x8
    u32 count;           // 0x10
};

void fn_7101059a58(UnkPathPointList* self, u32 index) {
    if (index < self->count) {
        self->data[index].Destroy();
        for (u64 i = index; i < self->count - 1; ++i) {
            self->data[i].vtable = reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10);
            self->data[i].field_0x8 = self->data[i + 1].field_0x8;
            self->data[i].field_0xc = self->data[i + 1].field_0xc;
            self->data[i].field_0x10 = self->data[i + 1].field_0x10;
            self->data[i].field_0x18 = self->data[i + 1].field_0x18;
            self->data[i + 1].Destroy();
        }
        --self->count;
    }
}
