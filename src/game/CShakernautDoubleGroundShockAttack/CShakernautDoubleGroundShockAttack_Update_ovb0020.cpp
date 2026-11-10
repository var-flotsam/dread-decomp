// CShakernautDoubleGroundShockAttack unit (tools/tu_map.py unit 1041): the attack's update (fn_71006a515c): after
// the base update (fn_71003609a8), states 5 and 6 count their timer (+0x60) up to the delay at +0xec of +0x28; then
// the timer restarts and state 5 moves to 7, state 6 to 8, each handing its id (g_unk_7172705810, g_unk_7172705818)
// to fn_71006a4d74. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_7172705810 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705818 __attribute__((visibility("hidden")));

struct UnkShakernautTiming_ovb0020 {
    u8 unk_0x0[0xec];
    f32 delay;
};

struct UnkShakernautShockAttack_ovb0020 {
    u8 unk_0x0[0x28];
    UnkShakernautTiming_ovb0020* timing;
    u32 state;  // 0x30
    u8 unk_0x34[0x2c];
    f32 timer;  // 0x60
};

void fn_71006a4d74(UnkShakernautShockAttack_ovb0020* self, const CStrId* id, bool flag);

void fn_71006a515c(UnkShakernautShockAttack_ovb0020* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    u32 state = self->state;
    if (state != 5 && state != 6)
        return;
    self->timer += dt;
    if (!(self->timer >= self->timing->delay))
        return;
    self->timer = 0.0f;
    if (state == 5) {
        self->state = 7;
        CStrId id(g_unk_7172705810);
        fn_71006a4d74(self, &id, false);
    } else {
        self->state = 8;
        CStrId id(g_unk_7172705818);
        fn_71006a4d74(self, &id, false);
    }
}
