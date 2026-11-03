// CGooshockerAIComponent unit (tools/tu_map.py, the name from reflection): after the base step
// (fn_7100599e0c) it binds the callback fn_710059f1e0 to the component, both in its own block at +0x148c8
// and at +0x19d0 of the owner's (+0x30) object at +0x218, resets that block's state and counters and sets
// the flag at +0x146db. Names and layouts are placeholders.
#include "types.h"

void fn_7100599e0c(char* self);
void fn_710059f1e0();

struct UnkGooshockerCallback_b54 {
    void* object;
    void (*fn)();
};

struct UnkGooshockerBlock_b54 {
    u32 state;
    u8 pad_0x4[4];
    UnkGooshockerCallback_b54 callback;  // 0x8
    u16 field_0x18;
    u8 pad_0x1a[2];
    u32 field_0x1c;
    u16 field_0x20;
};

struct UnkGooshockerTarget_b54 {
    u8 pad_0x0[0x19d0];
    UnkGooshockerCallback_b54 callback;
};

struct UnkGooshockerOwner_b54 {
    u8 pad_0x0[0x218];
    UnkGooshockerTarget_b54* target;
};

struct UnkGooshockerAI_b54 {
    u8 pad_0x0[0x30];
    UnkGooshockerOwner_b54* owner;
    u8 pad_0x38[0x146db - 0x38];
    bool flag;  // 0x146db
    u8 pad_0x146dc[0x148c8 - 0x146dc];
    UnkGooshockerBlock_b54 block;  // 0x148c8
};

void fn_710059e298(UnkGooshockerAI_b54* self) {
    UnkGooshockerBlock_b54* block = &self->block;
    fn_7100599e0c(reinterpret_cast<char*>(self));
    UnkGooshockerTarget_b54* target = self->owner->target;
    block->callback.object = self;
    block->callback.fn = &fn_710059f1e0;
    target->callback.object = self;
    target->callback.fn = &fn_710059f1e0;
    block->state = 0;
    block->field_0x18 = 1;
    block->field_0x1c = 0;
    self->flag = true;
    block->field_0x20 = 0;
}
