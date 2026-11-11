// CInfesterAIComponent's unit (tools/tu_map.py, unit 892): fn_71005cbcb4 picks the side to act on
// (fn_71005cbe00) from where the target is, and fn_71005cb1e4 leaves combat after a while, clearing the
// blackboard flags "bInCombat" and "bFleeBeforePush". Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/CEnemyMovement.h"
#include "game/CFrozenComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;

extern CType g_unk_71726ec448 __attribute__((visibility("hidden")));  // CInfesterAIComponent's type

struct UnkInfesterOwner_ovb0130 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x1f0 - 0x48];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x20];
    void* field_0x218;
    u8 pad_0x220[0x20];
    UnkReflObject* ai;  // 0x240
};

struct UnkInfesterState_ovb0130 {
    u8 pad_0x0[0x518];
    u32 field_0x518;
};

struct UnkInfesterAI_ovb0130 {
    u8 pad_0x0[0x30];
    UnkInfesterOwner_ovb0130* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkInfesterState_ovb0130* field_0x146c8;
};

bool fn_71005cbe00(UnkInfesterAI_ovb0130* self, bool right, UnkReflObject* movement);

bool fn_71005cbcb4(UnkInfesterAI_ovb0130* self) {
    UnkReflObject* o = self->owner->ai;
    if (!o)
        return false;
    const CType* type = o->vf_0x0();
    fn_71005ca678();
    bool is_ai = fn_710009d50c(type, &g_unk_71726ec448);
    if (self->field_0x146c8->field_0x518 != 0 || !is_ai)
        return false;
    if (fn_71010cc3a4(self->owner->field_0x218) != 1)
        return false;
    UnkInfesterOwner_ovb0130* target =
        reinterpret_cast<UnkInfesterOwner_ovb0130*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)));
    f32 tx = target->x;
    f32 ox = self->owner->x;
    UnkReflObject* mv = self->owner->movement;
    UnkReflObject* movement = mv && fn_710009d50c(mv->vf_0x0(), fn_71010c74e0()) ? mv : nullptr;
    if (ox + 10.0f >= tx && ox - 10.0f <= tx) {
        if (fn_71005cbe00(self, false, movement))
            return true;
        return fn_71005cbe00(self, true, movement);
    }
    if (ox < tx)
        return fn_71005cbe00(self, true, movement);
    return fn_71005cbe00(self, false, movement);
}

using base::global::CStrId;

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

extern base::global::CStrId g_unk_71726b7cb8;

struct UnkInfesterOwner2_ovb0130 {
    u8 pad_0x0[0x2c8];
    void* field_0x2c8;
};

struct UnkInfesterAI2_ovb0130 {
    u8 pad_0x0[0x30];
    UnkInfesterOwner2_ovb0130* owner;  // 0x30
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1260[0x146d4 - 0x1260 - sizeof(CBlackboard)];
    f32 combat_time;  // 0x146d4
    f32 timer;        // 0x146d8
    bool field_0x146dc;
};
static_assert(__builtin_offsetof(UnkInfesterAI2_ovb0130, combat_time) == 0x146d4, "layout");

// Stores `value` under `name` in the blackboard section g_unk_71726b7cb8 names (unless the blackboard
// is locked).
static inline void SetBool_ovb0130(CBlackboard* blackboard, const char* key, bool value) {
    CStrId name(key, true);
    UnkTypedValue tv;
    bool v = value;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            tv.type = fn_710000d228();
            tv.value = &v;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &tv);
        }
    }
}

void fn_71005cb1e4(UnkInfesterAI2_ovb0130* self, f32 dt) {
    if (void* c = self->owner->field_0x2c8) {
        if (fn_71007f9f20(c))
            return;
    }
    f32 t = self->timer + dt;
    self->timer = t;
    if (!(t >= self->combat_time))
        return;
    SetBool_ovb0130(&self->blackboard, "bInCombat", false);
    SetBool_ovb0130(&self->blackboard, "bFleeBeforePush", false);
    self->timer = 0.0f;
    self->field_0x146dc = true;
}
