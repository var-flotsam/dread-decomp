#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CFrozenComponent.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU500.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/CAbilityShinespark.h"
#include "game/behaviortree/CBehavior.h"

using base::global::CStrId;

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};


struct UnkTimerOwner_r {
    u8 pad_0x0[0x2c8];
    UnkFrostbiteComponent* field_0x2c8;
};
struct UnkTimerCfg_r {
    u8 pad_0x0[0x494];
    f32 field_0x494;
};
struct UnkTimerRamp_r {
    f32 field_0x0;
    f32 field_0x4;
    bool field_0x8;
    bool field_0x9;
    u16 field_0xa;
};
struct UnkTimerSelf_r {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkTimerOwner_r* owner;
    u8 pad_0x38[6];
    bool field_0x3e;
    u8 pad_0x3f[0x1260 - 0x3f];
    CBlackboard blackboard;
};

struct UnkTimerSelf2_r {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkTimerOwner_r* owner;
    u8 pad_0x38[6];
    bool field_0x3e;
    u8 pad_0x3f[0x1260 - 0x3f];
    u8 blackboard[0xf0];
    u8 pad_0x1350[0x1610 - 0x1350];
    UnkTimerCfg_r* field_0x1610;
    u8 pad_0x1618[0x1466c - 0x1618];
    UnkTimerRamp_r ramp;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7101110e18(UnkTimerSelf2_r* self, f32 dt) {
    UnkFrostbiteComponent* comp = self->owner->field_0x2c8;
    if (comp && fn_71007f9f20(comp) && !fn_71007f9fd0(comp))
        return;
    f32 v;
    {
        f32 stored = -1.0f;
        CBlackboard* bb = reinterpret_cast<CBlackboard*>(self->blackboard);
        if (CBlackboard::CSection* s = fn_7101047270(bb, g_unk_71726b7cb8)) {
            fn_7100395b34(s, &g_unk_7172a06338, &stored);
            v = stored;
        } else {
            v = -1.0f;
        }
    }
    f32 next = v > 0.001f ? v - dt : v;
    if (next <= 0.001f)
        next = 0.0f;
    CBlackboard* bb = reinterpret_cast<CBlackboard*>(self->blackboard);
    UnkTimerRamp_r* r = &self->ramp;
    // keeps the window bounds computed after the clamp
    asm("" : "+w"(v), "+w"(next));
    f32 lo = v + -0.001f;
    f32 hi = v + 0.001f;
    if (!(hi >= next && lo <= next)) {
        UnkTypedValue tv;
        f32 out = next;
        if (CBlackboard::CSection* s = fn_71010472e0(bb, g_unk_71726b7cb8)) {
            if (!bb->field_0xe8) {
                tv.type = fn_710000abbc();
                tv.value = &out;
                fn_7100332c80(s, &g_unk_7172a06338, &tv);
            }
        }
    }
    if (self->field_0x3e) {
        f32 t = r->field_0x0;
        if (!r->field_0x9) {
            t = t - dt;
            r->field_0x0 = t;
            if (!(t <= 0.0f))
                goto after;
        } else if (!(t <= 0.0f)) {
            goto after;
        }
        reinterpret_cast<void (*)(UnkTimerSelf2_r*)>(self->vtable[0x438 / 8])(self);
        r->field_0x8 = true;
        self->field_0x3e = false;
    }
after:
    if (r->field_0x8) {
        r->field_0x4 = r->field_0x4 + dt;
        if (r->field_0x4 > self->field_0x1610->field_0x494) {
            r->field_0x8 = false;
            r->field_0xa = 0;
            r->field_0x4 = 0.0f;
        }
    }
}
