// CChozoCommanderLandingSlamAttack's unit (tools/tu_map.py unit 654): the attack's update. After the base update
// (fn_7100424b5c) it compares the owner's (+0x8) x (+0x44) with that of the actor fn_7100d7c990 returns. In mode 0
// (+0x58) the timer (+0x5c) advances by dt; while it is within the definition's limit (+0xf4 of +0x50) and the two
// are more than 50 apart it runs fn_710043764c, else fn_7100437c44. In mode 1, with a CChozoCommanderAIComponent,
// an animation (+0x220) and a navigation object (+0x298), once the owner's y (+0x54) is at most 10 plus the
// height fn_7100977bec gives plus the AI's offset (+0x15d20), the navigation object gets a query (vtable
// 0x7101a24b58) facing the other actor's side, the animation a mode-0 request (as src/game_s2/UnkSpitSend.h), and
// the callback fn_71004385ac (link at +0x60, handle at +0x78) is hooked up to it as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp; then the mode becomes 2. Names and layouts
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CChozoCommanderLandingSlamAttack.h"
#include "game/UnkActor.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "game_s2/UnkChozoCommanderAttack.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkFlagged;

struct UnkSlamUpdate_A2_b75;
void fn_710043764c(UnkSlamUpdate_A2_b75* self);
void fn_71004385ac();
struct UnkSlamAttack_A2_b72;

struct UnkSlamDir_A2_b75 {
    f32 x, y, z;
};

struct UnkSlamQuery_A2_b75 {
    void** vtable;
    UnkSlamDir_A2_b75 field_0x8;
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;

    __attribute__((always_inline)) void Init() {
        field_0x1c = 0;
        field_0x34 = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        vtable = g_unk_7101a24b58 + 2;
        field_0x30 = 0;
    }
};

struct UnkSlamDef_A2_b75 {
    u8 pad_0x0[0xf4];
    f32 limit;
};

struct UnkSlamLink_A2_b75 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkSlamUpdate_A2_b75 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x40];
    UnkSlamDef_A2_b75* def;  // 0x50
    s32 mode;                // 0x58
    f32 timer;               // 0x5c
    UnkSlamLink_A2_b75 link;  // 0x60
    UnkWeakHandle* handle;    // 0x78
};

// The x position's bit pattern: the original keeps both in integer registers.
static inline u32 PosX(UnkActor* actor) {
    return *reinterpret_cast<u32*>(reinterpret_cast<char*>(actor) + 0x44);
}

static inline f32 AsF32(u32 bits) {
    f32 value;
    __builtin_memcpy(&value, &bits, 4);
    return value;
}

void fn_7100437930(UnkSlamUpdate_A2_b75* self, f32 dt) {
    fn_7100424b5c(reinterpret_cast<UnkChozoCommanderAttack*>(self), dt);
    u32 x = PosX(self->owner);
    u32 other = PosX(fn_7100d7c990(self->owner));
    switch (self->mode) {
    case 0: {
        f32 t = self->timer + dt;
        self->timer = t;
        if (!(t > self->def->limit) && !(__builtin_fabsf(AsF32(x) - AsF32(other)) <= 50.0f))
            fn_710043764c(self);
        else
            fn_7100437c44(reinterpret_cast<UnkSlamAttack_A2_b72*>(self));
        break;
    }
    case 1: {
        UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
        UnkReflObject* commander = ai && fn_710009d50c(ai->vf_0x0(), fn_710044e708()) ? ai : nullptr;
        UnkActor* owner = self->owner;
        asm("" : "+r"(commander));
        if (!commander)
            return;
        void* anim = owner->field_0x220;
        if (!anim)
            return;
        void* nav = owner->field_0x298;
        if (!nav)
            return;
        f32 h = fn_7100977bec(static_cast<UnkFlagged*>(owner->field_0x218)) + 10.0f;
        if (!(*reinterpret_cast<f32*>(reinterpret_cast<char*>(self->owner) + 0x54) <=
              h + *reinterpret_cast<f32*>(reinterpret_cast<char*>(commander) + 0x15d20)))
            return;
        // Declared past a goto (no lifetime markers), so each keeps its own slot.
        goto start;
        base::reflection::UnkReflTypeIdArg mode;
        UnkSlamQuery_A2_b75 query;
        base::reflection::UnkReflTypeIdArg mask;
    start:
        {
            f32 dir = AsF32(other) >= AsF32(x) ? 1.0f : -1.0f;
            query.Init();
            query.field_0x8.x = dir;
            query.field_0x8.y = 0.0f;
            query.field_0x8.z = 0.0f;
            query.field_0x14 = 1;
            fn_71006f4b0c(nav, &query, 0);
        }
        mode.id = 0;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(anim, 0xb4589aae3884b4be, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        UnkSlamLink_A2_b75* link = &self->link;
        link->context = self;
        link->callback = fn_71004385ac;
        UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        if (track) {
            asm("" : "+r"(self));
            fn_7100316018(track, &self->handle);
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }
        self->mode = 2;
        break;
    }
    }
}
