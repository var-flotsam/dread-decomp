// UnkTU811 (CEmmyAIComponent's unit): drives the EMMI's glow effect, pulsing its alpha with a sine while the
// glow is wanted and pausing between pulses, then hands the colour to the effect's material.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/CFXComponent.h"
#include "game/UnkTU811.h"
#include "game_s2/UnkChargeComponent.h"
#include "types.h"

#include <math.h>

struct UnkFactorEntity;
f32 fn_7100d7ce40(UnkFactorEntity* self);
extern const base::math::CVector4D g_unk_71016477b4;  // (0, 0, 0, 0)

// The time left before the next pulse, and the pulse's phase.
__attribute__((visibility("hidden"))) extern f32 g_unk_7101a48b48;
__attribute__((visibility("hidden"))) extern f32 g_unk_71726e0bc0;

struct UnkEmmyGlowTunable_l543888 {
    u8 pad_0x0[0x4c];
    f32 pulsePause;  // 0x4c
    f32 pulseSpeed;  // 0x50
    u8 pad_0x54[0x80 - 0x54];
    bool glowA;  // 0x80
    u8 pad_0x81[0xc6 - 0x81];
    bool glowB;  // 0xc6
};

struct UnkGlowMaterial_l543888 {
    u8 pad_0x0[0x30];
    base::math::CVector4D* color;  // 0x30
};

struct UnkGlowTarget_l543888 {
    u8 pad_0x0[0x14c];
    base::math::CVector4D color;  // 0x14c
    u8 pad_0x15c[0x1f0 - 0x15c];
    UnkGlowMaterial_l543888* material;  // 0x1f0
};

struct UnkGlowList_l543888 {
    u8 pad_0x0[0x48];
    UnkGlowTarget_l543888** targets;  // 0x48
};

struct UnkGlowModel_l543888 {
    u8 pad_0x0[0x78];
    UnkGlowList_l543888* list;  // 0x78
};

struct UnkGlowPart_l543888 {
    u8 pad_0x0[0xb];
    u8 index;  // 0xb
    u8 pad_0xc[0x160 - 0xc];
    UnkGlowModel_l543888* model;  // 0x160
};

struct UnkEmmyOwner_l543888 {
    u8 pad_0x0[0x280];
    UnkWorld* effects;  // 0x280
};

struct UnkEmmyTuning_l543888 {
    u8 pad_0x0[0x18];
    bool glow;  // 0x18
};

struct UnkEmmyContext_l543888 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual UnkEmmyTuning_l543888* vf_0xd8();  // the tuning block

    u8 pad_0x8[0x610 - 0x8];
    bool enabled;  // 0x610
};

struct UnkEmmyAI_l543888 {
    void** vtable;
    u8 pad_0x8[0x30 - 0x8];
    UnkEmmyOwner_l543888* owner;  // 0x30
    u8 pad_0x38[0x14c35 - 0x38];
    bool glowing;  // 0x14c35
    u8 pad_0x14c36[0x29ee0 - 0x14c36];
    UnkEmmyContext_l543888* context;  // 0x29ee0
    u8 pad_0x29ee8[0x4];
    s32 mode;  // 0x29eec

    bool vf_0x7d8() { return reinterpret_cast<bool (*)(UnkEmmyAI_l543888*)>(vtable[0x7d8 / 8])(this); }
    bool vf_0x7e0() { return reinterpret_cast<bool (*)(UnkEmmyAI_l543888*)>(vtable[0x7e0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEmmyAI_l543888, glowing) == 0x14c35, "UnkEmmyAI_l543888 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l543888, mode) == 0x29eec, "UnkEmmyAI_l543888 layout");

static const u64 kGlowEffect = 0x7ff940855c402436;

static inline bool IsViewing(UnkEmmyAI_l543888* self) {
    return fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self));
}

// Whether either of the two states the tunable picks, or the tuning in mode 1, asks for the glow.
static inline bool WantsGlow(UnkEmmyAI_l543888* self, const UnkEmmyGlowTunable_l543888* tunable) {
    bool stateA = self->vf_0x7e0();
    if (tunable->glowA && stateA)
        return true;
    bool stateB = self->vf_0x7d8();
    if (tunable->glowB && stateB)
        return true;
    return self->mode == 1 && self->context->vf_0xd8()->glow;
}

void fn_7100543888(UnkEmmyAI_l543888* self, f32 dt) {
    if (!self->context->enabled)
        return;

    base::math::CVector4D color;
    color.x = 2.0f;
    color.y = 0.7f;
    color.z = 0.2f;
    color.w = 0.0f;
    const UnkEmmyGlowTunable_l543888* tunable = reinterpret_cast<const UnkEmmyGlowTunable_l543888*>(&g_unk_71726e0aa8);
    bool glow = false;
    if (self->glowing) {
        f32 pause = tunable->pulsePause;
        f32 speed = tunable->pulseSpeed;
        if (IsViewing(self) && WantsGlow(self, tunable)) {
            glow = true;
            if (g_unk_7101a48b48 < 0.0f) {
                g_unk_71726e0bc0 += speed * (dt / fn_7100d7ce40(reinterpret_cast<UnkFactorEntity*>(self->owner)));
                f32 wave = sinf(g_unk_71726e0bc0);
                if (wave <= -1.0f)
                    color.w = -1.0f;
                else if (wave >= 1.0f)
                    color.w = 1.0f;
                else
                    color.w = wave;
                if (color.w < 0.0f) {
                    g_unk_7101a48b48 = pause;
                    color.w = 0.0f;
                }
            } else {
                g_unk_7101a48b48 -= dt;
                color.w = 0.0f;
                if (pause > 0.0f && g_unk_7101a48b48 < 0.0f)
                    g_unk_71726e0bc0 = 0.0f;
            }
            f32 t = color.w;
            if (t >= 1.0f)
                t = 1.0f;
            else if (t < 0.0f)
                t = 0.0f;
            color.w = t * 0.24f + (1.0f - t) * 0.15f;
        }
    }
    if (!glow)
        g_unk_71726e0bc0 = 0.0f;

    UnkWorld* effects = self->owner->effects;
    fn_710114fe8c(effects, kGlowEffect, glow && IsViewing(self), 0);
    UnkGlowPart_l543888* part = reinterpret_cast<UnkGlowPart_l543888*>(fn_7101152b90(effects, kGlowEffect));
    if (!fn_710114e680(reinterpret_cast<UnkObject*>(effects), part->index))
        part = nullptr;
    UnkGlowTarget_l543888* target = *part->model->list->targets;
    const base::math::CVector4D* shown = IsViewing(self) ? &color : &g_unk_71016477b4;
    fn_710007b518(&target->color, shown);
    if (target->material)
        *target->material->color = *shown;
}
