#include "types.h"

void* fn_71001039f8();
struct UnkAtkLock_b0149;
void fn_71012392d0(UnkAtkLock_b0149* lock);
void fn_71012392e0(UnkAtkLock_b0149* lock);
void* fn_7101228c30();
struct UnkAtkMsg_b0149 {
    u8 pad_0x0[0x8];
    u32 field_0x8;
    u8 pad_0xc[0xc];
    u32 field_0x18;
};
UnkAtkMsg_b0149* fn_71012279f8(void* queue, u32 size, u32 align, u32 flag);
void fn_7101227fb8(void* queue, void* msg);
void fn_7101236ba0(void* item);

struct UnkAtkLink_b0149 {
    UnkAtkLink_b0149* prev;
    UnkAtkLink_b0149* next;
};
struct UnkAtkMixer_b0149 {
    u8 pad_0x0[0xe0];
    u32 values[1];  // 0xe0
};

void fn_7101214bd0(void* self_, u32 value, u32 index) {
    u8* self = (u8*)self_;
    u32* slot = (u32*)self + index + 0x38;
    if (__builtin_expect(*slot != value, 1)) {
    *slot = value;
    void* queue = fn_7101228c30();
    UnkAtkMsg_b0149* msg = fn_71012279f8(queue, 0x20, 8, 1);
    msg->field_0x18 = 8;
    msg->field_0x8 = 0x4d;
    fn_7101227fb8(queue, msg);
    if (self[0x950]) {
        UnkAtkLock_b0149* lock = (UnkAtkLock_b0149*)((u8*)fn_71001039f8() + 0x8a0);
        fn_71012392d0(lock);
        UnkAtkLink_b0149* head = (UnkAtkLink_b0149*)(self + 0x890);
        for (UnkAtkLink_b0149* n = head->next; n != head; n = n->next)
            fn_7101236ba0((u8*)n - 0x40);
        fn_71012392e0((UnkAtkLock_b0149*)((u8*)fn_71001039f8() + 0x8a0));
        fn_7101236ba0(self + 0x558);
    }
    }
}

struct UnkAtkVoiceItem_b0149 {
    u8 pad_0x0[0x20];
    u8 out[0x8];  // 0x20
    u8 field_0x28;
    u8 field_0x29;
    u8 pad_0x2a[0x7e];
    u32 field_0xa8;
    u8 pad_0xac[0x1c];
    struct UnkAtkWave_b0149* waves;  // 0xc8
    u64 pad_0xd0[2];
    u64 field_0xe0;
};
struct UnkAtkWave_b0149 {
    u8 pad_0x0[0x34];
    u32 state;  // 0x34
    UnkAtkWave_b0149* next;  // 0x38
};
struct UnkAtkVoicePool_b0149 {
    UnkAtkVoiceItem_b0149* items;  // 0x0
    UnkAtkVoiceItem_b0149** slots;  // 0x8
    s32 count;  // 0x10
    s32* index;  // 0x18
};
void fn_710123a680(void* sys, void* out);

void fn_7101230180(UnkAtkVoicePool_b0149* self, UnkAtkVoiceItem_b0149* item) {
    for (UnkAtkWave_b0149* w = item->waves; w; w = w->next)
        w->state = 3;
    item->pad_0xd0[0] = 0;
    item->pad_0xd0[1] = 0;
    item->waves = nullptr;
    if (item->field_0x29) {
        fn_710123a680((u8*)fn_71001039f8() + 0x18, item->out);
        item->field_0x29 = 0;
    }
    item->field_0xe0 = 0;
    item->field_0x28 = 0;
    item->field_0xa8 = 1;
    u64 base = (u64)self->items;
    u64 idx = ((u64)item - base) >> 8;
    s64 slot = self->index[idx];
    s32 last = self->count - 1;
    self->count = last;
    UnkAtkVoiceItem_b0149* moved = self->slots[last];
    __builtin_memcpy(&self->slots[slot], &moved, 8);
    __builtin_memcpy(&self->slots[last], &item, 8);
    s32 slot32 = slot;
    __builtin_memcpy(&self->index[((u64)moved - base) >> 8], &slot32, 4);
    s32 none = -1;
    __builtin_memcpy(&self->index[idx], &none, 4);
}
