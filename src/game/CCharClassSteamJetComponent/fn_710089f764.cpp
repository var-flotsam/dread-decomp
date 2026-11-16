// CCharClassSteamJetComponent's unit: for a time of zero or less, restarts the timer block at +0xc80 when
// a target is set (at +0xc58), else disables the component (fn_710024fa0c). Names and layouts are placeholders.
#include "game/CActorComponent.h"
#include "types.h"

struct UnkStruct_710024fa0c;

struct UnkSteamJet_l89f764 {
    u8 pad_0x0[0xc58];
    void* target;
    u8 pad_0xc60[0x20];
    u64 field_0xc80;
    u8 pad_0xc88[4];
    u16 field_0xc8c;
    u8 pad_0xc8e[2];
    f32 time;  // 0xc90
    f32 progress;  // 0xc94
    bool field_0xc98;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710089f764(f32 time, UnkSteamJet_l89f764* self) {
    if (!(time <= 0.0f))
        return;
    if (!self->target) {
        fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), false);
        return;
    }
    self->time = time;
    f32 progress = time >= 1.0f ? 0.0f : 1.0f;
    self->field_0xc98 = true;
    self->field_0xc80 = 0;
    self->field_0xc8c = 0;
    self->progress = progress;
}
