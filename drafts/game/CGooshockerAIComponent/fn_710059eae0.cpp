// CGooshockerAIComponent unit (tools/tu_map.py unit 854): a step of the shock. After fn_710059b324, a
// pending cancel (+0x146da) clears the shock; else a shock under way restarts its timer (+0x148c8 from
// the tunable at +0x148c0) and fades the playing shock sound or starts it; the shock is then marked on
// and a second timer (+0x148e4) is taken from the object at +0x146e0 when asked (+0x146db). Names and
// layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CGooshockerAIComponent.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkInterp_7100105d34;
void fn_71001059f0(UnkInterp_7100105d34* self, f32 time);

// The shock sound's file, set up with the unit.
extern CFilePathStrId g_unk_71726e7a20 __attribute__((visibility("hidden")));

struct UnkGooshockerSound_l59eae0 {
    u8 pad_0x0[0x118];
    bool field_0x118;
};

// The handle's view of the reference block.
struct UnkGooshockerSoundRef_l59eae0 {
    UnkGooshockerSound_l59eae0* sound;
    s64 serial;
};

static inline UnkGooshockerSound_l59eae0* Playing(const UnkSoundHandle& handle) {
    const UnkGooshockerSoundRef_l59eae0* ref = reinterpret_cast<const UnkGooshockerSoundRef_l59eae0*>(handle.ref);
    if (ref && ref->serial == handle.serial && handle.serial != -1)
        return ref->sound;
    return nullptr;
}

struct UnkGooshockerTunable_l59eae0 {
    u8 pad_0x0[0x6f8];
    f32 shock_time;  // 0x6f8
};

struct UnkGooshockerTimer_l59eae0 {
    void** vtable;
    f32 vf_0xa0(UnkEntityWithSound* owner, u32 arg) {
        return reinterpret_cast<f32 (*)(UnkGooshockerTimer_l59eae0*, UnkEntityWithSound*, u32)>(
            vtable[0xa0 / 8])(this, owner, arg);
    }
};

struct UnkGooshockerAI_l59eae0 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* owner;  // 0x30
    u8 pad_0x38[0x1140 - 0x38];
    u32 field_0x1140;
    u8 pad_0x1144[0x146c8 - 0x1144];
    bool field_0x146c8;
    u8 pad_0x146c9[0xf];
    bool field_0x146d8;
    u8 pad_0x146d9;
    bool field_0x146da;
    bool field_0x146db;
    u8 pad_0x146dc[0x4];
    UnkGooshockerTimer_l59eae0 timer;  // 0x146e0
    u8 pad_0x146e8[0x147f8 - 0x146e8];
    s32 field_0x147f8;
    u8 pad_0x147fc[0x148b0 - 0x147fc];
    UnkSoundHandle sound;  // 0x148b0
    UnkGooshockerTunable_l59eae0* tunable;  // 0x148c0
    f32 field_0x148c8;
    u8 pad_0x148cc[0x148e0 - 0x148cc];
    bool field_0x148e0;
    u8 pad_0x148e1[0x3];
    f32 field_0x148e4;
};
static_assert(__builtin_offsetof(UnkGooshockerAI_l59eae0, timer) == 0x146e0, "layout");
static_assert(__builtin_offsetof(UnkGooshockerAI_l59eae0, field_0x148e4) == 0x148e4, "layout");

void fn_710059b324(UnkGooshockerAI_l59eae0* self);
void fn_710059e8e0(UnkGooshockerAI_l59eae0* self);

void fn_710059eae0(UnkGooshockerAI_l59eae0* self) {
    bool active = self->field_0x146c8;
    fn_710059b324(self);
    if (self->field_0x146da) {
        self->field_0x146da = false;
        self->field_0x148e0 = false;
        return;
    }
    if (active && self->field_0x148e0) {
        fn_710059e8e0(self);
        self->field_0x148c8 = self->tunable->shock_time;
        if (UnkGooshockerSound_l59eae0* sound = Playing(self->sound)) {
            if (!sound->field_0x118)
                fn_71001059f0(reinterpret_cast<UnkInterp_7100105d34*>(sound), 0.2f);
        } else {
            static UnkSoundDesc s_desc;
            s_desc.kind.field_0x0 = 0;
            s_desc.path = g_unk_71726e7a20;
            s_desc.field_0x38 = 1.0f;
            s_desc.field_0x4c = 0.0f;
            s_desc.field_0x6c = true;
            s_desc.field_0x6d = true;
            self->sound = fn_7100d7c374(self->owner, &s_desc, &g_unk_7101d470e0, false, true, true);
        }
    }
    if (!self->field_0x148e0) {
        self->field_0x148e0 = true;
        if (self->field_0x147f8 <= 0)
            self->field_0x146d8 = true;
    }
    if (self->field_0x146db) {
        self->field_0x148e4 = self->timer.vf_0xa0(self->owner, self->field_0x1140);
        fn_710059e4f0(reinterpret_cast<UnkGooshockerState*>(self), false);
        self->field_0x146db = false;
    }
}
