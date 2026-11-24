// CNailongThornsAttack's unit (tools/tu_map.py): the attack's start. After the base start it keeps the owner's
// AI when it is a CNailongAIComponent (+0x60), sends the owner's character class one of two tracks (and, when
// a turn track plays, hands the owner's +0x298 a kind-5 parameter block), copies two values from the
// description (+0x28), and hooks the callback fn_710060dd9c (link at +0x30, handle at +0x48) up to the
// character class. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkAttack.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

void fn_710060dd9c();

// The kind-5 parameter block (game_s2/UnkParams.h) with its two optional values written out.
struct UnkThornsParams_l60b9b8 {
    void** vtable;
    u64 field_0x8;
    f32 speed;        // 0x10
    bool has_speed;   // 0x14
    u8 pad_0x15[3];
    f32 delay;        // 0x18
    bool has_delay;   // 0x1c
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 kind;         // 0x34
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkThornsDesc_l60b9b8 {
    u8 pad_0x0[0xec];
    bool field_0xec;
    u8 pad_0xed[0x118 - 0xed];
    u32 field_0x118;
};

struct UnkThornsLink_l60b9b8 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkNailongThornsAttack_l60b9b8 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x28 - 0x10];
    UnkThornsDesc_l60b9b8* desc;  // 0x28
    UnkThornsLink_l60b9b8 link;   // 0x30
    UnkWeakHandle* handle;        // 0x48
    u32 field_0x50;
    u32 pad_0x54;
    void* field_0x58;
    UnkReflObject* ai;            // 0x60
    u64 field_0x68;
    bool field_0x70;
    u8 pad_0x71[0x80 - 0x71];
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;
    u32 field_0x8c;
    bool field_0x90;
};
static_assert(__builtin_offsetof(UnkNailongThornsAttack_l60b9b8, field_0x90) == 0x90, "layout");

void fn_710060b9b8(UnkNailongThornsAttack_l60b9b8* self) {
    base::reflection::UnkReflTypeIdArg turn_mode;
    base::reflection::UnkReflTypeIdArg turn_mask;
    UnkThornsParams_l60b9b8 params;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x50 = 0;
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    self->ai = ai && fn_710009d50c(ai->vf_0x0(), fn_710061f464()) ? ai : nullptr;
    void* cls = self->owner->field_0x220;
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(cls), fn_7100d2b3a8())) {
        turn_mode.id = 10;
        turn_mask.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, 0xb3feb6d516806342, 0xf4061ff25a2163d6, &turn_mode, &g_unk_710164b958,
                      &g_unk_7172769740, &turn_mask);
        void* target = self->owner->field_0x298;
        params.field_0x8 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.field_0x30 = 0;
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.kind = 5;
        params.delay = 0.0f;
        params.speed = 1.0f;
        params.has_delay = true;
        params.has_speed = true;
        fn_71006f4b0c(target, &params, 1);
    } else {
        mode.id = 2;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, 0xcb24643fce5b5852, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
    }
    UnkThornsDesc_l60b9b8* desc = self->desc;
    self->field_0x70 = desc->field_0xec;
    self->field_0x68 = 8;
    self->field_0x84 = 0;
    self->field_0x88 = 0;
    self->field_0x8c = 1;
    self->field_0x90 = true;
    self->field_0x80 = desc->field_0x118;
    UnkActor* owner = self->owner;
    self->field_0x58 = owner->field_0x220;
    if (owner->field_0x220) {
        UnkThornsLink_l60b9b8* link = &self->link;
        link->context = self;
        link->callback = fn_710060dd9c;
        UnkWeakTarget* track = static_cast<UnkWeakTarget*>(owner->field_0x220);
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
            CAbilityGhostAura_AssignWeakHandleToRef(track, &self->handle);
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }
    }
}
