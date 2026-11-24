// CBossAIComponent unit: the boss's update step. Unless the flag object at +0x14908 says otherwise, it
// refreshes the life bar at +0x148f0, runs the boss's timers and helpers and counts how long the owner
// stays near its landmark height, then runs the behaviour tree's step. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;

struct UnkBossColorAI_ovb0102;
struct UnkBossAITimers_ovb0102;
struct UnkBossBlinkAI_ovb0102;
struct UnkStruct_71003ae428;
struct UnkSelf_71003ad45c;
struct UnkRenderItemPair_b64a2;
void CBossAIComponent_UpdateColorChange(UnkBossColorAI_ovb0102* self, f32 dt, bool counting);
void CBossAIComponent_UpdateTimersAndRestoreDefaultCamera(UnkBossAITimers_ovb0102* self, f32 dt);
void CBossAIComponent_UpdateDamageFX(UnkBossBlinkAI_ovb0102* self, f32 dt);
void fn_710035d558(UnkRenderItemPair_b64a2* self);
void fn_710035d5c4(UnkRenderItemPair_b64a2* self, f32 value);
bool fn_71010e390c(void* menu, const CStrId* id);
bool fn_710110c4f8(void* self, bool flag);

inline const CStrId& LifeBarId_l3ae15c() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table.field_0x998;
}

struct UnkBossLife_l3ae15c {
    u8 pad_0x0[0x50];
    f32 max;
    f32 current;
};

struct UnkBossFlags_l3ae15c {
    u8 pad_0x0[0x50c];
    bool field_0x50c;
    bool field_0x50d;
    bool field_0x50e;
};

struct UnkBossHeight_l3ae15c {
    bool enabled;
    f32 y;
    u8 pad_0x8[0x8];
};

struct UnkBossAI_l3ae15c {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14700 - 0x38];
    f32 height_tolerance;              // 0x14700
    UnkBossHeight_l3ae15c heights[2];  // 0x14704: below, above
    f32 near_height_time;  // 0x14724
    u8 pad_0x14728[0x147e9 - 0x14728];
    bool field_0x147e9;
    u8 pad_0x147ea[0x2];
    f32 field_0x147ec;
    f32 field_0x147f0;
    bool field_0x147f4;
    u8 pad_0x147f5[0x14804 - 0x147f5];
    s32 field_0x14804;
    u8 pad_0x14808[0x148d8 - 0x14808];
    bool field_0x148d8;
    f32 field_0x148dc;
    u8 pad_0x148e0[0x148f0 - 0x148e0];
    u8 life_bar[0x18];  // 0x148f0
    UnkBossFlags_l3ae15c* flags;  // 0x14908

    UnkRenderItemPair_b64a2* LifeBar() { return reinterpret_cast<UnkRenderItemPair_b64a2*>(life_bar); }
};
static_assert(__builtin_offsetof(UnkBossAI_l3ae15c, near_height_time) == 0x14724, "layout");
static_assert(__builtin_offsetof(UnkBossAI_l3ae15c, field_0x147f4) == 0x147f4, "layout");
static_assert(__builtin_offsetof(UnkBossAI_l3ae15c, flags) == 0x14908, "layout");

// Whether the owner is within the tolerance of the height on the target's side (above or below it).
static inline bool NearHeight(UnkBossAI_l3ae15c* self, UnkActor* target) {
    if (!self->heights[0].enabled && !self->heights[1].enabled)
        return false;
    f32 y = self->owner->field_0x44;
    if (target->field_0x44 > y) {
        if (!self->heights[1].enabled)
            return false;
        return __builtin_fabsf(y - self->heights[1].y) < self->height_tolerance;
    }
    if (!self->heights[0].enabled)
        return false;
    return __builtin_fabsf(y - self->heights[0].y) < self->height_tolerance;
}

void fn_71003ae15c(UnkBossAI_l3ae15c* self, f32 dt) {
    if (self->flags->field_0x50c) {
        fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
        return;
    }
    if (self->owner->field_0x1b8 &&
        fn_71010e390c(fn_7100defcec(g_unk_7101d47310), &LifeBarId_l3ae15c())) {
        UnkRenderItemPair_b64a2* bar = self->LifeBar();
        UnkBossLife_l3ae15c* life = static_cast<UnkBossLife_l3ae15c*>(self->owner->field_0x1b8);
        if (life->max > 1.1920929e-07f)
            fn_710035d5c4(bar, life->current / life->max);
        else
            fn_710035d5c4(bar, 0.0f);
    } else {
        fn_710035d558(self->LifeBar());
    }
    self->field_0x148dc = self->field_0x148d8 ? self->field_0x148dc + dt : 0.0f;

    CBossAIComponent_UpdateColorChange(reinterpret_cast<UnkBossColorAI_ovb0102*>(self), dt, true);
    if (self->flags->field_0x50e && self->field_0x14804 >= 1) {
        if (fn_71003ad4dc(reinterpret_cast<UnkSelf_71003ad45c*>(self), 0.15f, self->field_0x14804))
            self->field_0x14804 = -1;
    }

    bool b = fn_710110c4f8(self, false);
    self->field_0x147e9 = b;
    if (b) {
        self->field_0x147ec += dt;
        if (self->field_0x147f0 != 0.0f)
            self->field_0x147f0 = 0.0f;
    } else {
        self->field_0x147f0 += dt;
        if (self->field_0x147ec != 0.0f)
            self->field_0x147ec = 0.0f;
    }

    CBossAIComponent_UpdateTimersAndRestoreDefaultCamera(reinterpret_cast<UnkBossAITimers_ovb0102*>(self), dt);
    fn_71003ae428(reinterpret_cast<UnkStruct_71003ae428*>(self), self->field_0x147f4, false, false);

    UnkActor* target = fn_7100d7c990(self->owner);
    bool near = target && NearHeight(self, target);
    if (near)
        self->near_height_time += dt;
    else
        self->near_height_time = 0.0f;

    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    CBossAIComponent_UpdateDamageFX(reinterpret_cast<UnkBossBlinkAI_ovb0102*>(self), dt);
}
