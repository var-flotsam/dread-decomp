// behaviortree::CInPathPatrolAction's unit (tools/tu_map.py, unit 488): the update step (fn_710038ce9c). Returns 3
// while the owner's movement is in state 4; otherwise plays the next animation when the timer ran out, hands the
// navigation a kind-5 parameter block, clears the AI's blackboard flag and returns 7. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAIComponent.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::global::CStrId;

struct UnkInPathPatrolAI_l38ce9c {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x13d2 - 0x1350];
    bool field_0x13d2;
};
static_assert(__builtin_offsetof(UnkInPathPatrolAI_l38ce9c, field_0x13d2) == 0x13d2, "layout");

// An animation key: the hash of a name.
struct UnkAnimKey_l38ce9c {
    u64 hash;
    explicit UnkAnimKey_l38ce9c(const CStrId& name) : hash(name.data->Key()) {}
};

// Play flags.
struct UnkAnimFlags_l38ce9c {
    u32 value;
    explicit UnkAnimFlags_l38ce9c(u32 v) : value(v) {}
};

// Plays the animation `anim` on the AI.
void fn_7101110964(UnkInPathPatrolAI_l38ce9c* ai, const CStrId* anim, const UnkAnimKey_l38ce9c* key,
                   const UnkAnimFlags_l38ce9c* flags, f32 a, f32 b);

// Passes a temporary by address.
template <class T>
static inline const T* Ptr(const T& v) {
    return &v;
}

struct UnkInPathPatrolOwner_l38ce9c {
    u8 pad_0x0[0x218];
    void* movement;  // 0x218
    u8 pad_0x220[0x240 - 0x220];
    UnkAIComponent* ai;  // 0x240
};

struct UnkInPathPatrol_l38ce9c {
    u8 pad_0x0[0x18];
    UnkInPathPatrolOwner_l38ce9c* owner;  // 0x18
    u8 pad_0x20[0x10];
    UnkInPathPatrolAI_l38ce9c* ai;  // 0x30
    u8 pad_0x38[0x70 - 0x38];
    CStrId next_anim;  // 0x70
    u8 pad_0x78[0x98 - 0x78];
    bool field_0x98;
    u8 pad_0x99;
    bool has_anim;  // 0x9a
    u8 pad_0x9b[0xa0 - 0x9b];
    f32 timer;  // 0xa0
};
static_assert(__builtin_offsetof(UnkInPathPatrol_l38ce9c, timer) == 0xa0, "layout");

// Reads the flag `name` in the section `section` into `out`; false when the section or the flag is missing.
static inline bool ReadFlag_l38ce9c(CBlackboard* blackboard, const CStrId& section, const CStrId& name, bool* out) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        return fn_710006d11c(s, &name, out);
    return false;
}

// Stores the flag `value` points to under `name` in the section `section`, unless the blackboard is locked.
static inline void SetFlag_l38ce9c(CBlackboard* blackboard, const CStrId& section, const CStrId& name,
                                   bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<CStrId*>(&name)), &v);
        }
    }
}

u32 fn_710038ce9c(UnkInPathPatrol_l38ce9c* self) {
    if (void* movement = self->owner->movement)
        if (fn_71010cc3a4(movement) == 4)
            return 3;
    if (self->has_anim && self->timer > 0.0f) {
        self->timer = -1.0f;
        fn_7101110964(self->ai, &self->next_anim,
                      Ptr(UnkAnimKey_l38ce9c(base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x218])),
                      Ptr(UnkAnimFlags_l38ce9c(0)), -1.0f, 0.0f);
    }
    if (UnkInPathPatrolOwner_l38ce9c* owner = self->owner) {
        if (UnkAIComponent* ai = owner->ai) {
            if (self->field_0x98) {
                if (UnkNavigation* navigation = fn_71010fccf8(ai)) {
                    UnkParams params;
                    params.field_0x1c = 0;
                    __builtin_memset(params.field_0x20, 0, 16);
                    params.field_0x30 = 0;
                    params.vtable = g_unk_7101a25b10 + 2;
                    __builtin_memset(params.field_0x8, 0, 13);
                    params.field_0x39 = 0;
                    params.field_0x3b = 0;
                    params.field_0x3d = 0;
                    params.field_0x34 = 5;
                    fn_71006f4b0c(navigation, &params, 0);
                    self->ai->field_0x13d2 = false;
                }
            }
        }
    }
    bool set = false;
    if (UnkInPathPatrolAI_l38ce9c* ai = self->ai) {
        CBlackboard* blackboard = &ai->blackboard;
        if (ReadFlag_l38ce9c(blackboard, g_unk_71726b7cb8,
                             base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x11e], &set) &&
            set) {
            const CStrId& name = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x11e];
            bool off = false;
            SetFlag_l38ce9c(blackboard, g_unk_71726b7cb8, name, &off);
        }
    }
    return 7;
}
