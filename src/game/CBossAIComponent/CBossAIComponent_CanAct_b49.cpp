// CBossAIComponent (tools/tu_map.py): whether the boss may act. It needs the target behind the handle
// at +0x14760 (resolved first in state 1 through fn_710023b580 on +0x14730, as in fn_71003b0160.cpp),
// no boss camera in control, and the owner's (+0x30) life (+0x1b8) present and not in state 2 (+0x70).
// While its +0x14908 flag (+0x50d) is set and the owner passes fn_7100d7b184 outside a CBoss* state of
// its animation (+0x220), slot 0x810 must agree too; the answer is then slot 0x7c8. Names and layouts
// are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkBossLife_b49 {
    u8 pad_0x0[0x70];
    s32 state;
};

struct UnkBossOwner_b49 {
    u8 pad_0x0[0x1b8];
    UnkBossLife_b49* life;
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* animation;
};
bool fn_7100d7b184(UnkBossOwner_b49* owner);

struct UnkBossSettings_b49 {
    u8 pad_0x0[0x50d];
    bool field_0x50d;
};

struct UnkBossTargetHolder_b49 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkBossAICanAct_b49 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBossOwner_b49* owner;
    u8 pad_0x38[0x146f8];
    u8 field_0x14730[0x28];
    u32 state;
    u8 pad_0x1475c[4];
    base::UnkRefHandle handle;
    u8 pad_0x14768[0x1a0];
    UnkBossSettings_b49* settings;

    bool vf_0x7c8() { return reinterpret_cast<bool (*)(UnkBossAICanAct_b49*)>(vtable[0x7c8 / 8])(this); }
    bool vf_0x810() { return reinterpret_cast<bool (*)(UnkBossAICanAct_b49*)>(vtable[0x810 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkBossAICanAct_b49, settings) == 0x14908, "layout");

bool fn_71003ad694(UnkBossAICanAct_b49* self) {
    if (self->state == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->field_0x14730);
            fn_7100234f78(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->state = 2;
    }
    auto* holder = static_cast<UnkBossTargetHolder_b49*>(self->handle.ptr);
    if (!holder || !holder->field_0x10)
        return false;
    if (void* cameras = fn_7100defcec(g_unk_7101d47310)) {
        if (UnkCameraCtrl* camera = fn_71010e2080(cameras)) {
            if (fn_710009d50c(camera->vf_0x0(), fn_710074eca8()))
                return false;
        }
    }
    UnkBossOwner_b49* owner = self->owner;
    if (!owner->life || owner->life->state == 2)
        return false;
    if (self->settings->field_0x50d && fn_7100d7b184(owner) &&
        !fn_710117b348(self->owner->animation, fn_7100cfa1a8()) && !self->vf_0x810())
        return false;
    return self->vf_0x7c8();
}
