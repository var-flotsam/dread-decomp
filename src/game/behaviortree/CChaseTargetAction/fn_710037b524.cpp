// Per-frame step of a component: runs the base step, then when the owner (+0x18) has an object at +0x240
// that fn_71010fccf8 turns into a target, hands a default parameter block to fn_71006f4b0c with 1 and,
// if the flag at +0x5a is set, passes the object to fn_71010fa828.
#include "game/UnkAIComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkParams.h"

struct UnkOwner {
    u8 pad_0x0[0x240];
    UnkAIComponent* field_0x240;
};

struct UnkComponent {
    u8 pad_0x0[0x18];
    UnkOwner* field_0x18;
    u8 pad_0x20[0x3a];
    u8 field_0x5a;
};

void fn_71010fa828(UnkAIComponent* object);

void fn_710037b524(UnkComponent* self) {
    fn_7100394bd4(self);
    if (self->field_0x18) {
        UnkAIComponent* object = self->field_0x18->field_0x240;
        if (object) {
            if (UnkNavigation* target = fn_71010fccf8(object)) {
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
            if (self->field_0x5a)
                fn_71010fa828(object);
        }
    }
}
