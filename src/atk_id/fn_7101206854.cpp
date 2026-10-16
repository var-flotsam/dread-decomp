// nn::atk::SoundActor vtable slot 0x18: forwards its argument to the owned object when
// active, else -1.
#include "types.h"

struct UnkAtkTarget {
    virtual void unk_vf0();
    virtual void unk_vf1();
    virtual void unk_vf2();
    virtual s32 unk_vf3(void* arg);
};

struct UnkSoundActor {
    u8 unk_0x0[0x8];
    UnkAtkTarget* target;
    u8 unk_0x10[0xbc];
    bool active;
};

s32 fn_7101206854(UnkSoundActor* self, void* arg) {
    if (self->active)
        return self->target->unk_vf3(arg);
    return -1;
}
