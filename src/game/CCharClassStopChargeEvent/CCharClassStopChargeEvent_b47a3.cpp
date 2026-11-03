// CCharClassStopChargeEvent unit (tools/tu_map.py): stops the charge of the component at +0x2a8 of the
// actor (fn_710083e33c), passing the event's flag at +0xc only while the charge (+0x17c) has reached its
// threshold (+0x174) and +0x178 is set. Names and layouts are placeholders.
#include "types.h"

struct UnkChargeComponent_b47a3 {
    u8 unk_0x0[0x174];
    f32 threshold;
    bool charging;
    u8 unk_0x179[3];
    f32 charge;
};

struct UnkChargeActor_b47a3 {
    u8 unk_0x0[0x2a8];
    UnkChargeComponent_b47a3* charge;
};

struct UnkStopChargeEvent_b47a3 {
    u8 unk_0x0[0xc];
    u8 flag;
};

void fn_710083e33c(UnkChargeComponent_b47a3* charge, bool flag);

void fn_7100ce29e0(UnkStopChargeEvent_b47a3* self, UnkChargeActor_b47a3** actor) {
    if (!*actor)
        return;
    UnkChargeComponent_b47a3* charge = (*actor)->charge;
    if (!charge)
        return;
    bool flag = false;
    if (charge->charging && charge->charge >= charge->threshold)
        flag = self->flag != 0;
    fn_710083e33c(charge, flag);
}
