// Resets a component: marks its two ids as unset, tells the target of the weak handle at +0x58 to forget
// the listener at +0x40 and drops the handle, clears two flags, runs the base reset (fn_710051db44),
// hands a default parameter block to the owner's target (+0x298) and, if the owner's object at +0x1f0 is
// an enemy movement, clears the two words at +0x19e0.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::CClass;

struct UnkMover {
    void** vtable;
    u8 pad_0x8[0x19d8];
    u64 field_0x19e0;
    u64 field_0x19e8;
};

struct UnkOwner {
    u8 pad_0x0[0x1f0];
    UnkMover* field_0x1f0;
    u8 pad_0x1f8[0xa0];
    void* field_0x298;
};

struct UnkComponent {
    u8 pad_0x0[0x18];
    UnkOwner* field_0x18;
    u8 pad_0x20[0x10];
    u64 field_0x30;
    u64 field_0x38;
    u8 field_0x40[0x18];
    UnkWeakHandle* field_0x58;
    u8 pad_0x60[4];
    u8 field_0x64;
    u8 pad_0x65[0x12a8c];
    u8 field_0x12af1;
};

void fn_710051db44(UnkComponent* self, u32 arg);
void fn_710117b3f0(UnkWeakTarget* target, void* object);

void fn_71005174e0(UnkComponent* self, u32 arg) {
    self->field_0x30 = ~0ull;
    self->field_0x38 = ~0ull;
    if (UnkWeakHandle* handle = self->field_0x58) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x40);
        ResetRef(&self->field_0x58);
    }
    self->field_0x12af1 = 0;
    self->field_0x64 = 0;
    fn_710051db44(self, arg);
    if (void* target = self->field_0x18->field_0x298) {
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
    UnkMover* mover = self->field_0x18->field_0x1f0;
    if (mover) {
        if (fn_710009d50c(reinterpret_cast<const CClass* (*)(UnkMover*)>(mover->vtable[0])(mover), fn_71010c74e0())) {
            mover->field_0x19e8 = 0;
            mover->field_0x19e0 = 0;
        }
    }
}
