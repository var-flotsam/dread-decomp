// CQuetzoaChargeAttack's unit (tools/tu_map.py): starts the charge. When the owner (+0x8) has its
// animation component (+0x220) and its navigation (+0x298) and the track 0x350329412d24f034 is not playing
// (fn_710117ae38), that track is sent with mode 0 (UnkSpitSend, src/game_s2/UnkSpitSend.h); a kind-5
// parameter block (game_s2/UnkParams.h) facing +1 or -1 by the sign of the owner's transform element 2 goes
// to the navigation (fn_71006f4b0c); and when the owner's AI component (+0x240) is a CQuetzoaXAIComponent
// it gets fn_71006407e0 with 1 and the owner's timeline (+0x2e8) g_unk_710164a3a8 set to the all-flags
// mask. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkParams.h"
#include "types.h"

void fn_71006407e0(base::reflection::UnkReflObject* self, bool value);

struct UnkQuetzoaChargeKey_b60a4;
extern UnkQuetzoaChargeKey_b60a4 g_unk_710164a3a8;  // reached through the GOT

struct UnkQuetzoaChargeFacing_b60a4 {
    f32 x, y, z;
    bool flag;
};

struct UnkQuetzoaChargeOwner_b60a4 {
    u8 pad_0x0[0x38];
    u8 xform[0x220 - 0x38];  // 0x38
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    base::reflection::UnkReflObject* field_0x240;
    u8 pad_0x248[0x298 - 0x248];
    void* field_0x298;
    u8 pad_0x2a0[0x2e8 - 0x2a0];
    UnkTimelineComponent* field_0x2e8;
};

struct UnkQuetzoaChargeAttack_b60a4 {
    u8 pad_0x0[0x8];
    UnkQuetzoaChargeOwner_b60a4* owner;
};

void fn_7100644780(UnkQuetzoaChargeAttack_b60a4* self) {
    UnkAnimationComponent* anim = self->owner->field_0x220;
    void* nav = self->owner->field_0x298;
    if (!anim || !nav || fn_710117ae38(anim, 0x350329412d24f034, false))
        return;
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg timeline_mask;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    UnkParams params;
start:
    mode.id = 0;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(anim, 0x350329412d24f034, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                  &mask);
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x30 = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->xform));
    UnkQuetzoaChargeFacing_b60a4* facing = reinterpret_cast<UnkQuetzoaChargeFacing_b60a4*>(params.field_0x8);
    facing->x = m.m[2] >= 0.0f ? 1.0f : -1.0f;
    facing->y = 0.0f;
    facing->z = 0.0f;
    facing->flag = true;
    fn_71006f4b0c(nav, &params, 0);
    base::reflection::UnkReflObject* ai = self->owner->field_0x240;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710063efe4())) {
        fn_71006407e0(ai, true);
        if (UnkTimelineComponent* timeline = self->owner->field_0x2e8) {
            timeline_mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a3a8), &timeline_mask.id, false, -1.0f);
        }
    }
}
