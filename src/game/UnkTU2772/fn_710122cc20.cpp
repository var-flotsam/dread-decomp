// nn::atk unit: constructor of a voice-like object: two sub-objects built with fn_7101230248, then
// cleared fields, six (1, 1) pairs, an owner pointer and an empty self-linked list at +0x288.
// Names and layouts are placeholders.
#include "types.h"

char* fn_7101230248(char* p0);

struct UnkAtkOne2 {
    f32 x;
    f32 y;
};

struct UnkAtkInitLink {
    UnkAtkInitLink* prev;
    UnkAtkInitLink* next;
};

struct UnkAtkInitObj {
    u8 pad_0x000[0x1c4];
    u32 field_0x1c4;
    u64 field_0x1c8;
    u8 pad_0x1d0[8];
    u32 field_0x1d8;
    u8 pad_0x1dc[0x1e8 - 0x1dc];
    u16 field_0x1e8;
    u8 pad_0x1ea[0x214 - 0x1ea];
    UnkAtkOne2 field_0x214;
    UnkAtkOne2 field_0x21c;
    UnkAtkOne2 field_0x224;
    UnkAtkOne2 field_0x22c;
    UnkAtkOne2 field_0x234;
    UnkAtkOne2 field_0x23c;
    u8 pad_0x244[0x260 - 0x244];
    void* field_0x260;
    u8 pad_0x268[0x278 - 0x268];
    u32 field_0x278;
    u8 pad_0x27c[4];
    u64 field_0x280;
    UnkAtkInitLink list;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710122cc20(UnkAtkInitObj* self, void* owner) {
    fn_7101230248(reinterpret_cast<char*>(self));
    fn_7101230248(reinterpret_cast<char*>(self) + 0xe0);
    self->field_0x1c4 = 0;
    self->field_0x1c8 = 0;
    self->field_0x1e8 = 0;
    self->field_0x1d8 = 0;
    self->field_0x224 = {1.0f, 1.0f};
    self->field_0x21c = {1.0f, 1.0f};
    self->field_0x214 = {1.0f, 1.0f};
    self->field_0x23c = {1.0f, 1.0f};
    self->field_0x234 = {1.0f, 1.0f};
    self->field_0x22c = {1.0f, 1.0f};
    self->field_0x260 = owner;
    self->field_0x278 = 0;
    self->field_0x280 = 0;
    self->list.prev = &self->list;
    self->list.next = &self->list;
}
