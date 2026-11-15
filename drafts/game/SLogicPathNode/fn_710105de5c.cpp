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
    UnkPathPoint* data;
    u32 count;  // 0x8
};

void fn_710105de5c(UnkPathPointList* self, const u32* index) {
    u32 idx = *index;
    if (idx < self->count) {
        UnkPathPoint* first = &self->data[idx];
        first->Destroy();
        for (u64 i = first - self->data; i < self->count - 1; ++i) {
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
