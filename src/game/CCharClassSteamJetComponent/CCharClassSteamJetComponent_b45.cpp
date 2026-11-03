// CCharClassSteamJetComponent's unit (tools/tu_map.py): from a time of at least 1.0, enables the
// component (fn_710024fa0c) and, with a target at +0xc58, starts the timer block at +0xc80 with
// that time. Names and layout are placeholders.
#include "types.h"

struct UnkStruct_710024fa0c;
void fn_710024fa0c(UnkStruct_710024fa0c*, bool);

struct UnkSteamJet_b45a2 {
    u8 pad_0x0[0xc58];
    void* target;
    u8 pad_0xc60[0x20];
    u64 field_0xc80;
    u8 pad_0xc88[4];
    u16 field_0xc8c;
    u8 pad_0xc8e[2];
    f32 time;  // 0xc90
    u32 field_0xc94;
    bool field_0xc98;
};

void fn_710089f708(f32 time, UnkSteamJet_b45a2* self) {
    if (!(1.0f <= time))
        return;
    fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), true);
    if (!self->target)
        return;
    self->field_0xc98 = true;
    self->field_0xc80 = 0;
    self->time = time;
    self->field_0xc94 = 0;
    self->field_0xc8c = 1;
}
