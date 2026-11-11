// CPlayerMovement unit: resetting the player's movement state.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkTU2689.h"
#include "types.h"

struct UnkPlayerMovement_c4;
struct UnkMovementTimer_c4;
struct UnkMovementTarget_c4;
// The base reset (not decompiled yet).
void fn_71009786a8(UnkPlayerMovement_c4* self, bool flag);
// Clears the timer at +0x708 (not decompiled yet).
void fn_710006c434(UnkMovementTimer_c4* timer);
// Sends the id to the target at +0x220 of the owner (not decompiled yet).
void fn_710117a110(UnkMovementTarget_c4* target, const base::global::CStrId* id);

struct UnkMovementFlag_c4 {
    u8 pad_0x0[0x60];
    u32 value;
};

struct UnkMovementOwner_c4 {
    u8 pad_0x0[0x220];
    UnkMovementTarget_c4* target;
    u8 pad_0x228[0x238 - 0x228];
    UnkMovementFlag_c4* flag;
};

struct UnkPlayerMovement_c4 {
    void (**vtable)(UnkPlayerMovement_c4*);
    u8 pad_0x8[0x30 - 0x8];
    UnkMovementOwner_c4* owner;
    u8 pad_0x38[0x7c - 0x38];
    u32 field_0x7c;
    u8 pad_0x80[0x6e8 - 0x80];
    u64 field_0x6e8;
    u32 field_0x6f0;
    u8 pad_0x6f4[0x700 - 0x6f4];
    u8 field_0x700;
    u8 pad_0x701[0x708 - 0x701];
    u8 timer[0x7b8 - 0x708];
    u16 field_0x7b8;
    u8 field_0x7ba;
    u8 pad_0x7bb[0x7c8 - 0x7bb];
    u32 field_0x7c8;
    u8 pad_0x7cc[0x7e4 - 0x7cc];
    bool field_0x7e4;
};

static inline const base::global::CStrId& ResetId_c4() {
    return *reinterpret_cast<const base::global::CStrId*>(
        reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) + 0x1d78);
}

void fn_71010d7b88(UnkPlayerMovement_c4* self, u32 flags) {
    fn_71009786a8(self, flags & 1);
    if (UnkMovementFlag_c4* flag = self->owner->flag)
        flag->value = 1;
    self->field_0x7c8 = 0;
    self->field_0x6e8 = 0;
    self->field_0x6f0 = 0;
    self->field_0x7c = 0;
    fn_710006c434(reinterpret_cast<UnkMovementTimer_c4*>(self->timer));
    bool pending = self->field_0x7e4;
    self->field_0x700 = 0;
    self->field_0x7b8 = 0;
    self->field_0x7ba = 0;
    self->field_0x7e4 = false;
    if (pending)
        self->vtable[127](self);
    if (flags & 1) {
        if (UnkMovementTarget_c4* target = self->owner->target)
            fn_710117a110(target, &ResetId_c4());
    }
}
