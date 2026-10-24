// Per-frame step of a component: runs the base step, then when the owner (+0x18) has an object at +0x240
// that fn_71010fccf8 turns into a target, builds a parameter block (vtable g_unk_7101a25b10, kind 5) on
// the stack and hands it to fn_71006f4b0c with 1.
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkOwner {
    u8 pad_0x0[0x240];
    void* field_0x240;
};

struct UnkComponent {
    u8 pad_0x0[0x18];
    UnkOwner* field_0x18;
};


void fn_7100394bd4(UnkComponent* self);
void* fn_71010fccf8(void* object);

void fn_7100381764(UnkComponent* self) {
    fn_7100394bd4(self);
    if (self->field_0x18 && self->field_0x18->field_0x240) {
        if (void* target = fn_71010fccf8(self->field_0x18->field_0x240)) {
            UnkParams params;
            __builtin_memset(params.field_0x8, 0, 13);
            params.field_0x30 = 0;
            __builtin_memset(params.field_0x20, 0, 16);
            params.vtable = g_unk_7101a25b10 + 2;
            params.field_0x39 = 0;
            params.field_0x3b = 0;
            params.field_0x3d = 0;
            params.field_0x34 = 5;
            params.field_0x18 = 0;
            params.field_0x1c = 1;
            fn_71006f4b0c(target, &params, 1);
        }
    }
}
