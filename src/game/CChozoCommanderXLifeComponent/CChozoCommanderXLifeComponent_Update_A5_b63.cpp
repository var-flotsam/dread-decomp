// CChozoCommanderXLifeComponent's unit (tools/tu_map.py): after the base update fn_7101160078, follows the
// wanted flag (+0x269) with the active flag (+0x268), setting a timeline id on the owner (+0x30, +0x2e8) when
// it turns on and removing it when it turns off. While active it clears the wanted flag, adds the time to
// +0x26c and puts the +0x240 object's +0xa8 (at least 0.1) into +0x7c of the tunable g_unk_7172790a08;
// otherwise it moves that value towards 1.3 at twice the time step. Then counts +0x274 down at twice the time
// step and reports the owner's +0x220 bag value (fn_710033187c). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "game/CLifeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkGlobal2;
extern UnkGlobal2 g_unk_7172790a08;
extern u64 g_unk_710164aff8;
extern char g_unk_71727302a8[] __attribute__((visibility("hidden")));  // the bag key

struct UnkCommanderLife_A5_b63;


struct UnkCommanderOwner_A5_b63 {
    u8 pad_0x0[0x220];
    char* field_0x220;
    u8 pad_0x228[0x2e8 - 0x228];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkCommanderTunable_A5_b63 {
    u8 pad_0x0[0xa8];
    f32 field_0xa8;
};

struct UnkCommanderLife_A5_b63 {
    u8 pad_0x0[0x30];
    UnkCommanderOwner_A5_b63* owner;
    u8 pad_0x38[0x240 - 0x38];
    UnkCommanderTunable_A5_b63* tunable;  // 0x240
    u8 pad_0x248[0x268 - 0x248];
    u8 active;  // 0x268 (a flag)
    u8 wanted;  // 0x269 (a flag)
    u8 pad_0x26a[2];
    f32 field_0x26c;
    u8 pad_0x270[4];
    f32 field_0x274;
};

static inline f32& Rate_A5_b63() {
    return *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_7172790a08) + 0x7c);
}

void fn_71008be25c(UnkCommanderLife_A5_b63* self, f32 dt) {
    fn_7101160078(self, dt);
    if (!self->active) {
        u8 wanted = self->wanted;
        if (!wanted) {
            asm("" : "+r"(wanted));  // the original stores the loaded flag, not a zero
            self->active = wanted;
            goto off;
        }
        UnkTimelineComponent* timeline = self->owner->timeline;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &g_unk_710164aff8, &mask.id, false, -1.0f);
        self->active = self->wanted;
        if (!self->active)
            goto off;
    } else {
        if (!self->wanted)
            fn_7100a2b7b8(self->owner->timeline, &g_unk_710164aff8);
        self->active = self->wanted;
        if (!self->active)
            goto off;
    }
    {
        self->wanted = 0;
        self->field_0x26c += dt;
        f32 rate = self->tunable->field_0xa8;
        if (rate <= 0.1f)
            rate = 0.1f;
        else
            rate = 1.3f < rate ? 1.3f : rate;
        Rate_A5_b63() = rate;
        goto done;
    }
off : {
    f32 rate = Rate_A5_b63();
    f32 lo = rate - 0.001f;
    f32 hi = rate + 0.001f;
    if (!(hi >= 1.3f && lo <= 1.3f)) {
        rate = (dt + dt) + rate;
        Rate_A5_b63() = rate;
        if (rate > 1.3f)
            Rate_A5_b63() = 1.3f;
    }
}
done:
    self->field_0x274 -= dt + dt;
    if (char* bag = self->owner->field_0x220)
        unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(bag + 0x80),
                                    *reinterpret_cast<const base::global::CStrId*>(g_unk_71727302a8),
                                    self->field_0x274);
}
