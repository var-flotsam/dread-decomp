// Unit 2702: an AI component resets its state once, then re-registers its owner unless the owner's movement
// says otherwise. Names and layouts are placeholders.
#include "base/UnkEmptyDtorElem.h"
#include "game/CInputComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkTU108.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkResetAI_l0fa940;
struct UnkResetVtable_l0fa940 {
    void* pad_0x0[0x7b];
    void (*vf_0x3d8)(UnkResetAI_l0fa940* self);
};

struct UnkResetMovement_l0fa940 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkResetOwner_l0fa940 {
    u8 pad_0x0[0x1b4];
    bool field_0x1b4;
    u8 pad_0x1b5[0x3b];
    UnkResetMovement_l0fa940* movement;  // 0x1f0
};

struct UnkResetAI_l0fa940 {
    UnkResetVtable_l0fa940* vtable;
    u8 pad_0x8[0x28];
    UnkResetOwner_l0fa940* owner;  // 0x30
    u8 pad_0x38[0x30];
    u8 field_0x68[0x8];
    u32 field_0x70;
    u8 pad_0x74[0x1c];
    u32 field_0x90;
    bool field_0x94;
    bool field_0x95;
    u8 pad_0x96[0x22];
    u32 field_0xb8;
    u8 pad_0xbc[0x1c];
    u8* field_0xd8;  // 4-byte elements
    u32 field_0xe0;
    u8 pad_0xe4[0x1c];
    u32 field_0x100;
    u8 pad_0x104[0x1004];
    u64 field_0x1108;
    u8 pad_0x1110[0x5d6];
    bool field_0x16e6;
    u8 pad_0x16e7[0x1c1];
    u8 field_0x18a8[1];
};

struct UnkComponent_7100286798;

void fn_71010fa940(UnkResetAI_l0fa940* self) {
    if (self->field_0x16e6) {
        self->field_0x16e6 = false;
        return;
    }
    bool locked = self->field_0x94;
    u32 state = self->field_0x70;
    if (state == ((state != 0) & !locked))
        return;
    UnkTileOutline* outline = reinterpret_cast<UnkTileOutline*>(self->field_0x68);
    self->field_0x94 = false;
    self->field_0x95 = true;
    self->field_0x70 = 0;
    self->field_0x90 = 0;
    self->field_0xb8 = 0;
    for (u64 i = 0; i < self->field_0xe0; ++i)
        fn_71002874dc(self->field_0xd8 + i * 4);
    self->field_0xe0 = 0;
    self->field_0x100 = 0;
    fn_7100119f98(outline, false);
    self->field_0x1108 = 0;
    fn_7100874460(self->field_0x18a8, reinterpret_cast<UnkWeakTarget**>(&self->owner));
    UnkResetOwner_l0fa940* owner = self->owner;
    if (!owner->field_0x1b4)
        return;
    UnkResetMovement_l0fa940* movement = owner->movement;
    if (!movement || movement->field_0xf9)
        return;
    if (fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(movement)))
        return;
    self->vtable->vf_0x3d8(self);
}
