// CDropterAIComponent unit (tools/tu_map.py unit 780): the per-frame step. After the base step it runs the
// dropter's phases (+0x1486c): waiting to drop (or settling after a hit), fading the first effect into the
// second, fading the second out and finishing. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCollisionComponent.h"
#include "game/CDropterAIComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU252.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector4D;

struct UnkItemBase_71011a91c4;
struct UnkSceneObjectSub;
struct UnkDropterHitAI_ovb0131;
void fn_71004ec714(UnkDropterHitAI_ovb0131* self, bool flag);

extern const u64 g_unk_7101649cc8;  // reached through the GOT

bool fn_710110583c(void* component);
void fn_7101160768(void* component);

struct UnkDropterEffectMaterial {
    u8 pad_0x0[0x30];
    CVector4D* params;  // 0x30
};

struct UnkDropterEffectInstance {
    u8 pad_0x0[0x17c];
    CVector4D color;  // 0x17c
    u8 pad_0x18c[0x208 - 0x18c];
    UnkDropterEffectMaterial* material;  // 0x208
};
static_assert(__builtin_offsetof(UnkDropterEffectInstance, material) == 0x208, "UnkDropterEffectInstance layout");

struct UnkDropterEffect_l4ecb8c {
    u8 pad_0x0[0x8];
    u8 item[0x128];  // 0x8, what the render manager releases
    void* model;  // 0x130
};

// What the drop effect's part gives for its first layer (vtable slot 0xf8).
struct UnkDropterPartLayer_l4ecb8c {
    u8 pad_0x0[0x15c];
    CVector4D color;  // 0x15c
    u8 pad_0x16c[0x1f8 - 0x16c];
    UnkDropterEffectMaterial* material;  // 0x1f8
};
static_assert(__builtin_offsetof(UnkDropterPartLayer_l4ecb8c, material) == 0x1f8, "layout");

struct UnkDropterPart_l4ecb8c {
    void** vtable;
    UnkDropterPartLayer_l4ecb8c* Layer(u32 index) {
        return reinterpret_cast<UnkDropterPartLayer_l4ecb8c* (*)(UnkDropterPart_l4ecb8c*, u32)>(
            vtable[0xf8 / 8])(this, index);
    }
};

// The kind-5 parameter block with its scale and two flags set.
struct UnkDropterParams_l4ecb8c {
    void** vtable;
    u64 field_0x8;
    f32 scale;  // 0x10
    u8 field_0x14;
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkDropterTunables_l4ecb8c {
    u8 pad_0x0[0x518];
    f32 wait_time;  // 0x518
    f32 fade_in_time;  // 0x51c
    f32 fade_out_time;  // 0x520
};

struct UnkDropterOwner_l4ecb8c {
    u8 pad_0x0[0x1b8];
    void* field_0x1b8;
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    void* field_0x240;
    u8 pad_0x248[0x38];
    UnkWorld* effects;  // 0x280
    u8 pad_0x288[0x10];
    void* field_0x298;
    void* collision;  // 0x2a0
    u8 pad_0x2a8[0x40];
    UnkTimelineComponent* timeline;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkDropterOwner_l4ecb8c, collision) == 0x2a0, "layout");

struct UnkDropterBlock_l4ecb8c {
    UnkDropterTunables_l4ecb8c* tunables;
    u8 pad_0x8[0x124];
    u32 state;  // 0x12c
    f32 timer;  // 0x130
    u8 pad_0x134[4];
    UnkDropterEffect_l4ecb8c* effect_a;  // 0x138
    UnkDropterEffect_l4ecb8c* effect_b;  // 0x140
    s32 count;  // 0x148
    bool counted;  // 0x14c
    bool dying;  // 0x14d
    bool dying_hit;  // 0x14e
};
static_assert(__builtin_offsetof(UnkDropterBlock_l4ecb8c, count) == 0x148, "layout");

struct UnkDropterAI_l4ecb8c {
    u8 pad_0x0[0x30];
    UnkDropterOwner_l4ecb8c* owner;  // 0x30
    u8 pad_0x38[0x14738 - 0x38];
    UnkDropterBlock_l4ecb8c block;  // 0x14738
};

void fn_71004ed238(UnkDropterAI_l4ecb8c* self);

static const u64 kDropEffect = 0x1865cf2096e1d55dull;

// Sets an effect instance's colour and its material's copy of it.
template <class T>
inline void SetColor(T* instance, const CVector4D& color) {
    base::math::Assign(instance->color, color);
    if (instance->material)
        *instance->material->params = color;
}

static inline UnkDropterEffectInstance* Instance(UnkDropterEffect_l4ecb8c* effect) {
    return reinterpret_cast<UnkDropterEffectInstance*>(fn_71001be428(effect->model));
}

static inline void Release(UnkDropterEffect_l4ecb8c*& effect) {
    if (effect) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(effect->item), 0);
        effect = nullptr;
    }
}

// Counts a step; every tenth runs fn_71004ed238 and starts the count again.
static inline void Count(UnkDropterAI_l4ecb8c* self, UnkDropterBlock_l4ecb8c* block) {
    block->counted = false;
    if (block->count == 10) {
        fn_71004ed238(self);
        block->counted = true;
        block->count = 1;
    }
    block->count++;
}

void fn_71004ecb8c(UnkDropterAI_l4ecb8c* self, f32 dt) {
    UnkDropterBlock_l4ecb8c* block = &self->block;
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    if (block->dying) {
        block->dying = false;
        fn_71008618cc(self->owner->collision);
    }
    switch (block->state) {
    case 0:
        // Declared in the switch body (no lifetime markers), so each keeps its own slot.
        base::reflection::UnkReflTypeIdArg hit_mode;
        base::reflection::UnkReflTypeIdArg hit_mask;
        {
        UnkDropterOwner_l4ecb8c* owner = self->owner;
        if (block->dying_hit) {
            UnkAnimationComponent* animation = owner->animation;
            UnkTrackQuery query;
            query.cls = fn_7100c40e38();
            query.result = nullptr;
            query.value = -1.0f;
            fn_710117b08c(animation, &query);
            if (fn_710110583c(self->owner->field_0x240)) {
                if (query.result)
                    return;
            }
            block->dying_hit = false;
            block->timer = block->tunables->wait_time;
            hit_mode.id = 2;
            hit_mask.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, 0xedd258757f06a48dull, 0xf4061ff25a2163d6ull, &hit_mode, &g_unk_710164b958,
                          &g_unk_7172769740, &hit_mask);
            UnkTimelineComponent* timeline = self->owner->timeline;
            base::reflection::UnkReflTypeIdArg value;
            value.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &g_unk_7101649cc8, &value.id, false, -1.0f);
            fn_710114fe8c(self->owner->effects, kDropEffect, 1, 0);
            return;
        }
        Count(self, block);
        void* target = self->owner->field_0x298;
        UnkDropterParams_l4ecb8c params;
        params.field_0x30 = 0;
        params.field_0x20 = 0;
        params.field_0x28 = 0;
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x8 = 0;
        params.kind = 5;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        params.scale = 1.0f;
        params.field_0x14 = 1;
        fn_71006f4b0c(target, &params, 1);
        f32 timer = block->timer - dt;
        block->timer = timer;
        f32 total = block->tunables->wait_time;
        if (auto* part = reinterpret_cast<UnkDropterPart_l4ecb8c*>(fn_7101152b90(self->owner->effects, kDropEffect))) {
            f32 scale = (total - timer) * 0.9f / total + 0.1f;
            UnkDropterPartLayer_l4ecb8c* layer = part->Layer(0);
            SetColor(layer, CVector4D{scale, 0.0f, 0.0f, 0.0f});
        }
        if (block->timer < 0.0f) {
            if (!block->counted) {
                fn_71004ed238(self);
                block->count = 1;
            }
            fn_71004ec714(reinterpret_cast<UnkDropterHitAI_ovb0131*>(self), true);
        }
        break;
    }
    case 1: {
        Count(self, block);
        block->timer -= dt;
        f32 total = block->tunables->fade_in_time;
        f32 t = (total - block->timer) / total;
        UnkDropterEffectInstance* first = Instance(block->effect_a);
        SetColor(first, CVector4D{t, 1.0f - t, 0.0f, 1.0f - t});
        UnkDropterEffectInstance* second = Instance(block->effect_b);
        SetColor(second, CVector4D{0.0f, 1.0f, 0.0f, t});
        if (block->timer < 0.0f) {
            if (!block->counted)
                fn_71004ed238(self);
            block->state = 2;
            block->timer = block->tunables->fade_out_time;
            fn_71001be020(reinterpret_cast<UnkSceneObjectSub*>(block->effect_a->model), g_unk_71726db108);
            Release(block->effect_b);
        }
        break;
    }
    case 2:
        base::reflection::UnkReflTypeIdArg end_mode;
        base::reflection::UnkReflTypeIdArg end_mask;
        {
        block->timer -= dt;
        f32 t = block->timer / block->tunables->fade_out_time;
        UnkDropterEffectInstance* instance = Instance(block->effect_a);
        SetColor(instance, CVector4D{1.0f - t, t, 0.0f, t});
        if (block->timer < 0.0f) {
            UnkAnimationComponent* animation = self->owner->animation;
            end_mode.id = 2;
            end_mask.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, 0x9333bd6838920193ull, 0xf4061ff25a2163d6ull, &end_mode, &g_unk_710164b958,
                          &g_unk_7172769740, &end_mask);
            Release(block->effect_a);
            block->state = 3;
            block->timer = 0.0f;
            fn_710114fe8c(self->owner->effects, kDropEffect, 0, 0);
            fn_7101160768(self->owner->field_0x1b8);
        }
        break;
    }
    default:
        break;
    }
}
