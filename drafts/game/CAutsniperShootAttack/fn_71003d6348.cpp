// CAutsniperShootAttack's unit (tools/tu_map.py unit 576): the charge step of the shot. The charge's progress
// (timer over duration) grows one effect part and fades another, sets the charge sound's pitch and volume, fires
// a timeline event past 80% and, once full, enters state 2 and flags the AI. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CFXComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU284.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

void fn_7100208fa4(UnkXformNode* self, const UnkVec3f* v);

// As src/game/CAutsniperShootAttack/CAutsniperShootAttack_Modes_ovb0122.cpp spells them.
struct UnkSound;
struct UnkSoundRef {
    UnkSound* sound;
    s64 serial;
};
struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c590(UnkEntityWithSound* self, void* a);

// The charge sound's id.
extern u8 g_unk_71726bf830[] __attribute__((visibility("hidden")));
// The timeline event sent past 80% of the charge.
extern const u64 g_unk_71016492a0;

struct UnkSniperStaticZero_l3d6348 {
    u32 value;
    UnkSniperStaticZero_l3d6348() : value(0) {}
};

inline const UnkSniperStaticZero_l3d6348& UnkSniperStaticZero() {
    static UnkSniperStaticZero_l3d6348 s_value asm("g_unk_7101d57cc8");
    return s_value;
}

inline u32 UnkSniperDefaultMask() {
    static u32 s_mask asm("g_unk_7101d57cb8") = ~UnkSniperStaticZero().value & 0xff;
    return s_mask;
}

struct UnkSniperPart_l3d6348 {
    u8 pad_0x0[0xb];
    u8 kind;
    u8 pad_0xc[0x1c];
    UnkXformNode node;  // 0x28
};

struct UnkSniperAI_l3d6348 {
    u8 pad_0x0[0x27248];
    bool charged;
};

struct UnkSniperOwner_l3d6348 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240, a CAutsniperAIComponent
    u8 pad_0x248[0x38];
    UnkWorld* fx;  // 0x280
    u8 pad_0x288[0x60];
    UnkTimelineComponent* timeline;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkSniperOwner_l3d6348, timeline) == 0x2e8, "layout");

struct UnkSniperParams_l3d6348 {
    u8 pad_0x0[0xf0];
    f32 duration;  // 0xf0
    u8 pad_0xf4[0x10];
    f32 pitch_full;    // 0x104
    f32 pitch_start;   // 0x108
    f32 volume_full;   // 0x10c
    f32 volume_start;  // 0x110
};
static_assert(__builtin_offsetof(UnkSniperParams_l3d6348, volume_start) == 0x110, "layout");

struct UnkSniperShootAttack_l3d6348 {
    void** vtable;
    UnkSniperOwner_l3d6348* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkSniperParams_l3d6348* params;  // 0x28
    u8 pad_0x30[0x20];
    u32 state;  // 0x50
    u8 pad_0x54[0x3c];
    f32 timer;  // 0x90
};
static_assert(__builtin_offsetof(UnkSniperShootAttack_l3d6348, timer) == 0x90, "layout");

static inline UnkSoundHandle ChargeSound(UnkSniperShootAttack_l3d6348* self) {
    return fn_7100d7c590(reinterpret_cast<UnkEntityWithSound*>(self->owner), g_unk_71726bf830);
}

static inline bool Playing(const UnkSoundHandle& h) {
    return h.ref && h.serial != -1 && h.ref->serial == h.serial && h.ref->sound;
}

static inline UnkInterp_7100105d34* Get(const UnkSoundHandle& h) {
    if (h.ref && h.serial != -1 && h.ref->serial == h.serial)
        return reinterpret_cast<UnkInterp_7100105d34*>(h.ref->sound);
    return nullptr;
}

// from at t below 0, to at t of 1 or more, in between linearly.
static inline f32 Lerp(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + t * to;
}

void fn_71003d6348(UnkSniperShootAttack_l3d6348* self, f32 dt) {
    f32 progress = self->timer / self->params->duration;
    f32 squared = progress * progress;
    f32 scale = squared >= 1.0f ? 3.0f : squared * 3.0f + (1.0f - squared);
    UnkTimelineComponent* timeline = self->owner->timeline;
    UnkWorld* fx = self->owner->fx;
    if (fx) {
        UnkSniperPart_l3d6348* part = reinterpret_cast<UnkSniperPart_l3d6348*>(fn_7101152b90(fx, 0x8d42b5ae61e25fcaull));
        if (part)
            part = fn_710114e680(reinterpret_cast<UnkObject*>(fx), part->kind) ? part : nullptr;
        UnkVec3f v(scale, scale, scale);
        fn_7100207be8(&part->node, &v);
        fx = self->owner->fx;
    }

    f32 fade = (1.0f - progress) * 5.0f;
    f32 alpha = !(fade <= 0.0f) ? (!(fade >= 1.0f) ? fade : 1.0f) : 0.0f;
    if (fx) {
        UnkSniperPart_l3d6348* part = reinterpret_cast<UnkSniperPart_l3d6348*>(fn_7101152b90(fx, 0xb79a735ccb508b0eull));
        if (part && fn_710114e668(reinterpret_cast<UnkObject*>(fx), part->kind)) {
            UnkVec3f v(alpha, alpha, alpha);
            fn_7100208fa4(&part->node, &v);
        }
    }

    UnkSniperParams_l3d6348* params = self->params;
    f32 pitch = Lerp(params->pitch_start, params->pitch_full, progress);
    f32 volume = Lerp(params->volume_start, params->volume_full, progress);
    if (Playing(ChargeSound(self))) {
        SetSoundVoicePitchOverDuration(Get(ChargeSound(self)), pitch, 0.2f);
        fn_7100105d34(Get(ChargeSound(self)), volume, false, 0.2f);
    }

    self->timer += dt;
    f32 now = self->timer / self->params->duration;
    if (progress < 0.8f && now > 0.8f) {
        u32 mask = UnkSniperDefaultMask();
        fn_7100a2cb7c(timeline, &g_unk_71016492a0, &mask, false, -1.0f);
    }
    if (self->timer >= self->params->duration) {
        self->timer = 0.0f;
        if (self->state != 2) {
            self->state = 2;
            UnkReflObject* ai = self->owner->ai;
            UnkSniperAI_l3d6348* found = fn_710009d50c(ai->vf_0x0(), fn_71003da2e0())
                                             ? reinterpret_cast<UnkSniperAI_l3d6348*>(ai)
                                             : nullptr;
            found->charged = true;
        }
    }
}
