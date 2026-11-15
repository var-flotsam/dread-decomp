// Unit 2661 (CCutscene::CTunableCutscene): removes the element at an index from an array of 0x30-byte polymorphic elements:
// destroys it, then moves each later element down one place. Names and layouts are placeholders.
#include <new>

#include "game/UnkTU2661.h"
#include "types.h"

// 35 bytes copied as one block.
struct __attribute__((packed)) UnkElemData2661 {
    u8 bytes[35];
};

struct UnkElem2661 {
    void** vtable;
    UnkElemData2661 data;  // 0x8
    u8 pad_0x2b[0x30 - 0x2b];

    UnkElem2661(UnkElem2661&& other) : vtable((void**)((char*)g_unk_7101cac988 + 0x10)), data(other.data) {}
    void Destroy() { reinterpret_cast<void (*)(UnkElem2661*)>(vtable[0])(this); }
};

struct UnkElemArray2661 {
    UnkElem2661* data;
    u32 count;  // 0x8
};

void fn_710106ef8c(UnkElemArray2661* self, const u32* index) {
    u64 i = (u64)*index * 3 / 3;
    if ((u32)i < self->count) {
        self->data[(u32)i].Destroy();
        u32 last = self->count - 1;
        for (; i < last; last = self->count - 1) {
            new (&self->data[i]) UnkElem2661(static_cast<UnkElem2661&&>(self->data[i + 1]));
            self->data[i + 1].Destroy();
            ++i;
        }
        self->count = last;
    }
}
