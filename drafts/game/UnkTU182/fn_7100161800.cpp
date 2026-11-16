// Unit 182 of tools/tu_map.py (the graphics device layer): makes `object` the current one. Unless it
// already is, it is prepared, each of its 16 buffers in use is bound through a callback at the next
// binding index, and its two blocks are passed on. Names and layouts are placeholders.
#include "game/UnkTU172.h"
#include "game/UnkTU182.h"
#include "types.h"

void fn_7100161fd0(void* object);
s32 fn_710015e950(u32 index);
extern u64 (*g_unk_716e29b088)(void*);
extern void (*g_unk_716e29b818)(void* context, u32 binding, u64 address, u64 size);
extern void (*g_unk_716e29b800)(void* context, u32 count, void* block);
extern void (*g_unk_716e29b808)(void* context, u32 count, void* block);

// A buffer of the object: where it starts in the object's memory and its size.
struct UnkBindBuffer_l161800 {
    u8 pad_0x0[0x10];
    u32 offset;  // 0x10
    u32 size;    // 0x14
};

struct UnkBindObject_l161800 {
    u8 pad_0x0[0x1c];
    u32 total_size;  // 0x1c
    u8 pad_0x20[0x4];
    u8 flags;  // 0x24: 4, each buffer runs to the end of the memory
    u8 pad_0x25[0xb];
    u32 field_0x30;
    u8 pad_0x34[0x4];
    UnkBindBuffer_l161800* buffers[16];  // 0x38
    u8 field_0xb8[0x40];
    u8 field_0xf8[0x80];
    u8 memory[0x8];  // 0x178
};
static_assert(__builtin_offsetof(UnkBindObject_l161800, memory) == 0x178, "layout");

extern UnkBindObject_l161800* g_unk_716e34ae10;  // the current object
extern u32 g_unk_716e34ae0c;                      // the next binding index

void fn_7100161800(void* object) {
    UnkBindObject_l161800* self = static_cast<UnkBindObject_l161800*>(object);
    if (self) {
        if (g_unk_716e34ae10 == self)
            return;
        fn_7100161fd0(self);
        g_unk_716e34ae0c = 0;
        u64 base = g_unk_716e29b088(self->memory);
        for (u64 i = 0; i < 16; ++i) {
            UnkBindBuffer_l161800* buffer = self->buffers[i];
            if (buffer && fn_710015e950(i) >= 0) {
                if (self->flags & 4) {
                    u32 size = self->total_size - buffer->offset;
                    void* context = fn_7100153d50();
                    g_unk_716e29b818(context, g_unk_716e34ae0c, base + buffer->offset, size);
                } else {
                    void* context = fn_7100153d50();
                    g_unk_716e29b818(context, g_unk_716e34ae0c, base + buffer->offset, buffer->size);
                }
                ++g_unk_716e34ae0c;
            }
        }
        void* context = fn_7100153d50();
        g_unk_716e29b800(context, self->field_0x30 + 1, self->field_0xb8);
        context = fn_7100153d50();
        g_unk_716e29b808(context, g_unk_716e34ae0c, self->field_0xf8);
    }
    g_unk_716e34ae10 = self;
}
