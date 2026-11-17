// CFXComponent's unit: hands the effects over when the player changes, clearing the old player's glow and
// overlay and moving one named effect to whichever component the game's state picks.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CFXComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU132.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkFXComponent_b0052;

struct UnkFXPlayerScenes_pc {
    u8 pad_0x0[0x50];
    void* scene;  // 0x50
};

struct UnkFXPlayerOwnerData_pc {
    u8 pad_0x0[0x228];
    UnkFXPlayerScenes_pc* scenes;  // 0x228
};

struct UnkFXPlayerEffect_pc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void SetEnabled(bool on);  // 0x40
    u32 flags;  // 0x8; its top byte is the kind
    u8 pad_0xc[0x10d];
    u8 field_0x119;
};

struct UnkFXPlayerEntry_pc {
    UnkFXPlayerEffect_pc* effect;
    u8 pad_0x8[0x18];
};

struct UnkFXPlayerName_pc {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkFXPlayerEffects_pc {
    UnkFXPlayerEntry_pc* values;
    UnkFXPlayerName_pc** names;
    u32 count;

    UnkFXPlayerEntry_pc* End() { return &values[count]; }
    UnkFXPlayerEntry_pc* Find(u64 hash) {
        for (u64 i = 0; i < count; ++i)
            if (names[i]->hash == hash)
                return &values[i];
        return End();
    }
    UnkFXPlayerEntry_pc* Get(u64 hash) {
        UnkFXPlayerEntry_pc* p = Find(hash);
        return p == End() ? nullptr : p;
    }
};

struct UnkFXPlayerComponent_pc {
    u8 pad_0x0[0x30];
    UnkFXPlayerOwnerData_pc* owner;  // 0x30
    u8 pad_0x38[0x18];
    UnkFXPlayerEffects_pc effects;  // 0x50
    u8 pad_0x68[0x64];
    f32 glow;  // 0xcc
    u8 pad_0xd0[0x38];
    u32 field_0x108;
    u8 pad_0x10c[0x34];
    void* overlay_scene;  // 0x140
    u8 pad_0x148[0x28];
    void* scene;  // 0x170
};
static_assert(__builtin_offsetof(UnkFXPlayerComponent_pc, glow) == 0xcc, "UnkFXPlayerComponent_pc layout");
static_assert(__builtin_offsetof(UnkFXPlayerComponent_pc, field_0x108) == 0x108, "UnkFXPlayerComponent_pc layout");
static_assert(__builtin_offsetof(UnkFXPlayerComponent_pc, overlay_scene) == 0x140, "UnkFXPlayerComponent_pc layout");
static_assert(__builtin_offsetof(UnkFXPlayerComponent_pc, scene) == 0x170, "UnkFXPlayerComponent_pc layout");

struct UnkFXPlayerActor_pc {
    u8 pad_0x0[0x280];
    UnkFXPlayerComponent_pc* fx;  // 0x280
};

struct UnkFXPlayerState_pc {
    u8 pad_0x0[0xd8];
    f32 field_0xd8;
};

void fn_7101152f88(UnkFXComponent_b0052* self, f32 value);
// Defined in this unit; it reads the offset and keeps no pointer to it.
void fn_71011531cc(UnkFXComponent_b0052* self, u32 flag, __attribute__((noescape)) void* arg, f32 value);
void fn_710115351c(UnkFXComponent_b0052* component);

static const u64 kPlayerEffect = 0x444c7220d7fc64c4ULL;

// The overlay-model manager, created on first use.
static inline UnkModelManager* Manager() {
    UnkModelManager* m = g_unk_71726b0cd8;
    if (!m) {
        m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return m;
}

static inline void SwitchOff(UnkFXPlayerComponent_pc* self, u64 hash) {
    UnkFXPlayerEntry_pc* entry = self->effects.Get(hash);
    if (entry) {
        UnkFXPlayerEffect_pc* effect = entry->effect;
        // The effect is used below whatever it is, so its kind is read before the null test.
        bool attached = (effect != nullptr) & ((effect->flags & 0xff000000) == 0x4000000);
        if (attached && effect->field_0x119) {
            void* scene = self->owner->scenes->scene;
            if (scene)
                fn_710014580c(effect, scene);
        }
        effect->SetEnabled(false);
    }
}

void fn_7101152cc8(UnkFXPlayerComponent_pc* self, UnkFXPlayerActor_pc* player) {
    if (!player)
        return;
    UnkFXPlayerComponent_pc* old = player->fx;
    if (!old)
        return;

    if (old->glow > 1.1920929e-07f) {
        fn_7101152f88(reinterpret_cast<UnkFXComponent_b0052*>(self), old->glow);
        old->glow = 0.0f;
        if (old->overlay_scene) {
            fn_710014580c(Manager()->models[0], old->overlay_scene);
            old->overlay_scene = nullptr;
        }
    }
    if (old->scene) {
        base::math::CVector3D offset;
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        fn_71011531cc(reinterpret_cast<UnkFXComponent_b0052*>(self), 0, &offset, 0.0f);
        fn_710115351c(reinterpret_cast<UnkFXComponent_b0052*>(old));
    }

    UnkFXPlayerState_pc* state = static_cast<UnkFXPlayerState_pc*>(fn_7100df5440(g_unk_7101d47310));
    if (state->field_0xd8 > 0.0f) {
        SwitchOff(old, kPlayerEffect);
        self->field_0x108 = 0;
        fn_710114fe8c(reinterpret_cast<UnkWorld*>(self), kPlayerEffect, 1, 1);
    } else {
        SwitchOff(self, kPlayerEffect);
    }
    fn_7100000790("fxcallbacks.OnPlayerChange", g_unk_710162e2ed);
}
