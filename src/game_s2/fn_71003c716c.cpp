// Event handler: for an event of kind 3 whose payload's name hash (+0x30 of the object reached through
// +0x10 and +0x38) is 0xe3d785cc963782a5, runs two updates (fn_71003c4e64, fn_71003c5b10) and drops the weak
// handle at +0x147b0 after telling its target to forget the listener at +0x14798. Always reports handled.
#include "game_s2/UnkWeakHandle.h"

struct UnkNamed {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkInner {
    u8 pad_0x0[0x38];
    UnkNamed* field_0x38;
};

struct UnkMiddle {
    u8 pad_0x0[0x10];
    UnkInner* field_0x10;
};

struct UnkEvent {
    u32 field_0x0;
    u8 pad_0x4[4];
    UnkMiddle* field_0x8;
};

struct UnkSelf {
    u8 pad_0x0[0x14798];
    u8 field_0x14798[0x18];
    UnkWeakHandle* field_0x147b0;
};

void fn_71003c4e64(UnkSelf* self, u32 flag);
void fn_71003c5b10(UnkSelf* self, u32 flag);
void fn_710117b3f0(UnkWeakTarget* target, void* object);

bool fn_71003c716c(UnkSelf* self, UnkEvent* event) {
    if (event->field_0x0 == 3 && event->field_0x8->field_0x10->field_0x38->field_0x30 == 0xe3d785cc963782a5ull) {
        fn_71003c4e64(self, 0);
        fn_71003c5b10(self, 0);
        if (UnkWeakHandle* handle = self->field_0x147b0) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, self->field_0x14798);
            ResetRef(&self->field_0x147b0);
        }
    }
    return true;
}
