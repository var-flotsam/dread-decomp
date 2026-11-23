// behaviortree::CEmmyPatrolBaseAction's unit (tools/tu_map.py unit 803): the action's update. Counts down
// the timer at +0x118 (setting +0x11c when it runs out), restarts the patrol (or stops the navigation
// with a kind-5 block when that fails), lets the owner's CEmmyAIComponent react to the actor it has to act
// against, then, in AI state 2 or 1, enters the matching state and writes it to the blackboard section
// g_unk_71726b7cb8. Always returns 2 (running). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU811.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CEmmyChaseAction.h"
#include "game/behaviortree/CEmmyPatrolAction.h"
#include "game/behaviortree/CEmmyPatrolBaseAction.h"
#include "game/behaviortree/CEmmySearchingAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::global::CStrId;

struct UnkSelf;
struct UnkEmmyActor_b55;

extern char g_unk_71726df1a0[];

// The point the AI component is told to go to (fn_7100552d78).
struct UnkEmmyGoal_l515904 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

struct UnkEmmyAI_l515904 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1258];
    CBlackboard blackboard;  // 0x1260
    u8 pad_after_blackboard[0x14b48 - 0x1260 - sizeof(CBlackboard)];
    s32 state;  // 0x14b48
    u8 pad_0x14b4c[0x4];
    u32 goal_x;  // 0x14b50
    u32 goal_y;  // 0x14b54
    u8 pad_0x14b58[0x29eec - 0x14b58];
    u32 field_0x29eec;
    u8 pad_0x29ef0[0x35];
    bool field_0x29f25;
    u8 pad_0x29f26[0x2a250 - 0x29f26];
    u32 field_0x2a250;

    bool vf_0x7d8() {
        return reinterpret_cast<bool (*)(UnkEmmyAI_l515904*)>((*reinterpret_cast<void***>(this))[0x7d8 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkEmmyAI_l515904, state) == 0x14b48, "UnkEmmyAI_l515904 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l515904, field_0x2a250) == 0x2a250, "UnkEmmyAI_l515904 layout");

struct UnkPatrolOwner_l515904 {
    u8 pad_0x0[0x240];
    UnkEmmyAI_l515904* ai;
};

// An entry of the action's patrol list (fn_7100515ffc).
struct UnkPatrolEntry_l515904 {
    u8 pad_0x0[0x8];
    u32 field_0x8;
    u32 field_0xc;
};

struct UnkPatrolUpdate_l515904 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkPatrolOwner_l515904* owner;  // 0x18
    u8 pad_0x20[0xf0];
    u32 field_0x110;
    u32 field_0x114;
    f32 timer;  // 0x118
    bool field_0x11c;
    u8 pad_0x11d[0x3];
    u8 field_0x120[0x18];
    u8 entries[0x10];  // 0x138

    void vf_0xc8() { reinterpret_cast<void (*)(UnkPatrolUpdate_l515904*)>(vtable[0xc8 / 8])(this); }
};

// A callback bound to the action: the object and the function called with it.
struct UnkPatrolCallback_l515904 {
    UnkPatrolUpdate_l515904* self;
    void (*fn)(UnkPatrolUpdate_l515904*);
};

UnkPatrolEntry_l515904* fn_7100515ffc(void* entries, u32 index);
void fn_7100516468(UnkPatrolUpdate_l515904* self);
void fn_7100516474(UnkPatrolUpdate_l515904* self);
void* fn_710054abc4(UnkEmmyAI_l515904* ai, UnkActor* target);
bool fn_710054b120(UnkSelf* self, void* a1);
bool fn_710054b200(UnkEmmyAI_l515904* ai, const base::math::CVector2D* position);
void fn_71005472d0(UnkEmmyAI_l515904* ai);
bool fn_710054a798(UnkEmmyAI_l515904* ai);
void fn_7100552d78(UnkEmmyAI_l515904* ai, const UnkEmmyGoal_l515904* goal);

static inline UnkEmmyAI_l515904* EmmyAI(UnkPatrolUpdate_l515904* self) {
    UnkEmmyAI_l515904* ai = self->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        return ai;
    return nullptr;
}

static inline void EnterState(UnkEmmyAI_l515904* ai, char* state) {
    fn_7100544a34(reinterpret_cast<UnkEmmyActor_b55*>(ai), state, true);
}

u32 fn_7100515904(UnkPatrolUpdate_l515904* self, f32 dt) {
    UnkEmmyAI_l515904* ai = EmmyAI(self);
    if (!ai)
        return 2;
    void* navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(ai));
    if (self->timer != 0.0f) {
        f32 left = self->timer - dt;
        self->timer = left;
        if (left <= 0.0f) {
            self->timer = 0.0f;
            self->field_0x11c = true;
        }
    }
    if (navigation) {
        bool started = false;
        if (self->field_0x11c) {
            if (UnkEmmyAI_l515904* emmy = EmmyAI(self)) {
                UnkPatrolEntry_l515904* entry = fn_7100515ffc(self->entries, 0);
                entry->field_0x8 = self->field_0x110;
                entry->field_0xc = self->field_0x114;
                UnkPatrolCallback_l515904 a = {self, fn_7100516468};
                UnkPatrolCallback_l515904 b = {self, fn_7100516474};
                UnkPatrolCallback_l515904 none = {nullptr, nullptr};
                // fn_7100556c00 returns a flag here (declared elsewhere without it).
                using Start = bool (*)(UnkEmmyAI_l515904*, bool, const UnkPatrolCallback_l515904*,
                                       const UnkPatrolCallback_l515904*, u32, void*, const UnkPatrolCallback_l515904&);
                started = reinterpret_cast<Start>(fn_7100556c00)(emmy, true, &a, &b, 1, self->field_0x120, none);
            }
        } else {
            started = fn_710051564c(reinterpret_cast<UnkPatrolBase_b55*>(self), false);
        }
        if (!started) {
            UnkParams params;
            __builtin_memset(params.field_0x8, 0, 13);
            params.field_0x30 = 0;
            __builtin_memset(params.field_0x20, 0, 16);
            params.vtable = g_unk_7101a25b10 + 2;
            params.field_0x39 = 0;
            params.field_0x3b = 0;
            params.field_0x3d = 0;
            params.field_0x34 = 5;
            params.field_0x18 = 0;
            params.field_0x1c = 1;
            fn_71006f4b0c(navigation, &params, 0);
        }
    }
    if (UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))) {
        if (void* found = fn_710054abc4(ai, target)) {
            if (fn_710054b120(reinterpret_cast<UnkSelf*>(ai), found)) {
                f32 x = target->field_0x44;
                f32 y = target->field_0x54;
                f32 z = target->field_0x64;
                base::math::CVector3D position = {x, y, z};
                base::math::CVector2D flat;
                fn_710007b430(&flat, &position);
                fn_7100547428(reinterpret_cast<UnkEmmyActor_b55*>(ai), fn_710054b200(ai, &flat) ? 2 : 1, target,
                              false);
                fn_71005472d0(ai);
            } else {
                self->vf_0xc8();
                if (fn_710054a798(ai))
                    fn_7100547428(reinterpret_cast<UnkEmmyActor_b55*>(ai), 1, target, false);
            }
        }
    }
    s32 state = ai->state;
    if (state == 2 && !ai->field_0x29f25 && !ai->vf_0x7d8()) {
        EnterState(ai, g_unk_71726debd0);
        if (CBlackboard::CSection* s = fn_71010472e0(&ai->blackboard, g_unk_71726b7cb8)) {
            if (!ai->blackboard.field_0xe8) {
                UnkTypedValue v;
                v.type = fn_7100009658();
                v.value = g_unk_71726ded60;
                fn_7100332c80(s, &g_unk_71726debd8, &v);
            }
        }
        return 2;
    }
    if (state == 1 || state == 2) {
        UnkEmmyGoal_l515904 goal = {ai->goal_x, ai->goal_y, 0};
        fn_7100552d78(ai, &goal);
        EnterState(ai, g_unk_71726df1a0);
        bool searching = true;
        if (CBlackboard::CSection* s = fn_71010472e0(&ai->blackboard, g_unk_71726b7cb8)) {
            if (!ai->blackboard.field_0xe8) {
                UnkTypedValue v;
                v.type = fn_710000d228();
                v.value = &searching;
                fn_7100332c80(s, &g_unk_71726df1a8, &v);
            }
        }
        if (ai->field_0x29eec == 0)
            ai->field_0x2a250 = 0;
    }
    return 2;
}
