// CChozoCommanderAIComponent unit (tools/tu_map.py unit 665): the component's start. After the base start,
// sets and clears the owner's six model flags, copies the arena range (four values at +0x40 of the target
// fn_710110c438 reports, refreshed through its vf_0x98 when its +0x60 flag is set) to +0x15d1c, clears
// +0x15ec8, enters the mode the tunable asks for (+0x2c of g_unk_71726c91d0) and hooks the callback
// fn_710045696c (link at +0x15f08, handle at +0x15f20) up to the owner's animation (+0x220).
// Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern base::reflection::UnkTunable g_unk_71726c91d0;
extern char g_unk_71016496c0[];
extern char g_unk_71016496c8[];
extern char g_unk_71016496d0[];
extern char g_unk_71016496d8[];
extern char g_unk_71016496e0[];
extern char g_unk_71016496e8[];

void fn_710045696c();

struct UnkCoreXQuery_A2_b73 {
    u8 pad_0x0[0x8];
    UnkWeakHandle** field_0x8;
    u32 field_0x10;
};
UnkCoreXQuery_A2_b73* fn_710110c438(UnkCoreXAI_A2_b73* self);

struct UnkArenaSource_l450084 {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    u8 pad_0x50[0x10];
    bool field_0x60;

    void vf_0x98() { reinterpret_cast<void (*)(UnkArenaSource_l450084*)>(vtable[0x98 / 8])(this); }
};

struct UnkChozoTunable_l450084 {
    u8 pad_0x0[0x2c];
    s32 field_0x2c;  // the mode to start in (1 or 2; anything else starts in none)
};

struct UnkChozoOwner_l450084 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;  // animation
    UnkFlagTarget* field_0x228;  // model updater
    u8 pad_0x230[0x10];
    UnkCoreXAI_A2_b73* field_0x240;
};

struct UnkChozoLink_l450084 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkChozoState_l450084 {
    f32 range[4];  // 0x15d1c
    u8 pad_0x10[0x19c];
    u32 field_0x1ac;  // 0x15ec8
    u32 field_0x1b0;
};

struct UnkChozoCommanderAI_l450084 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkChozoOwner_l450084* owner;  // 0x30
    u8 pad_0x38[0x15ce4];
    UnkChozoState_l450084 state;  // 0x15d1c
    u8 pad_0x15ed0[0x38];
    UnkChozoLink_l450084 link;  // 0x15f08
    UnkWeakHandle* handle;      // 0x15f20

    void vf_0x208(s32 mode, bool immediate) {
        reinterpret_cast<void (*)(UnkChozoCommanderAI_l450084*, s32, bool)>(vtable[0x208 / 8])(this, mode,
                                                                                               immediate);
    }
};
static_assert(__builtin_offsetof(UnkChozoCommanderAI_l450084, state) == 0x15d1c, "");
static_assert(__builtin_offsetof(UnkChozoCommanderAI_l450084, link) == 0x15f08, "");
static_assert(__builtin_offsetof(UnkChozoCommanderAI_l450084, handle) == 0x15f20, "");

void fn_7100450084(UnkChozoCommanderAI_l450084* self) {
    UnkChozoState_l450084* state = &self->state;
    fn_71003acb5c(reinterpret_cast<UnkCoreXAI_A2_b73*>(self));
    if (UnkFlagTarget* model = self->owner->field_0x228) {
        fn_7101172764(model, g_unk_71016496c0);
        fn_7101172780(self->owner->field_0x228, g_unk_71016496c8);
        fn_7101172764(self->owner->field_0x228, g_unk_71016496d0);
        fn_7101172780(self->owner->field_0x228, g_unk_71016496d8);
        fn_7101172780(self->owner->field_0x228, g_unk_71016496e0);
        fn_7101172780(self->owner->field_0x228, g_unk_71016496e8);
    }
    if (UnkCoreXQuery_A2_b73* query = fn_710110c438(self->owner->field_0x240)) {
        if (query->field_0x10 == 1) {
            if (UnkWeakHandle* handle = *query->field_0x8) {
                if (auto* source = reinterpret_cast<UnkArenaSource_l450084*>(handle->target)) {
                    if (source->field_0x60)
                        source->vf_0x98();
                    state->range[0] = source->field_0x40;
                    state->range[1] = source->field_0x44;
                    state->range[2] = source->field_0x48;
                    state->range[3] = source->field_0x4c;
                }
            }
        }
    }
    state->field_0x1ac = 0;
    state->field_0x1b0 = 0;
    switch (reinterpret_cast<const UnkChozoTunable_l450084*>(&g_unk_71726c91d0)->field_0x2c) {
    case 1:
        self->vf_0x208(1, true);
        break;
    case 2:
        self->vf_0x208(2, true);
        break;
    }
    UnkWeakTarget* anim = self->owner->field_0x220;
    if (!anim)
        return;
    UnkChozoLink_l450084* link = &self->link;
    link->context = self;
    link->callback = fn_710045696c;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        ResetRef(&self->handle);
    }
    fn_7100316018(anim, &self->handle);
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
}
