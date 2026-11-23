// behaviortree::CInPathPatrolAction's unit (tools/tu_map.py, unit 488): the end step (fn_710038ba54). Runs the
// base end, hands the navigation a kind-5 parameter block, clears the owner's flag at +0x13d2 and, when the
// owner's blackboard flag is set, clears it; then drops the weak handle at +0xc0. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkInPathPatrolOwner_l38ba54 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x13d2 - 0x1350];
    bool field_0x13d2;
};
static_assert(__builtin_offsetof(UnkInPathPatrolOwner_l38ba54, field_0x13d2) == 0x13d2, "layout");

struct UnkInPathPatrol_l38ba54 {
    u8 pad_0x0[0x30];
    UnkInPathPatrolOwner_l38ba54* owner;  // 0x30
    void* navigation;                     // 0x38
    u8 pad_0x40[0x98 - 0x40];
    bool field_0x98;
    u8 pad_0x99[0xa8 - 0x99];
    u8 link[0x18];             // 0xa8
    UnkWeakHandle* watched;    // 0xc0
};
static_assert(__builtin_offsetof(UnkInPathPatrol_l38ba54, watched) == 0xc0, "layout");

// Reads the flag `name` in the section `section` into `out`; false when the section or the flag is missing.
static inline bool ReadFlag_l38ba54(CBlackboard* blackboard, const CStrId& section, const CStrId& name, bool* out) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        return fn_710006d11c(s, &name, out);
    return false;
}

// Stores the flag `value` points to under `name` in the section `section`, unless the blackboard is locked.
static inline void SetFlag_l38ba54(CBlackboard* blackboard, const CStrId& section, const CStrId& name,
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

void fn_710038ba54(UnkInPathPatrol_l38ba54* self) {
    fn_7100394bd4(self);
    if (void* navigation = self->navigation) {
        UnkParams params;
        params.field_0x1c = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        if (self->field_0x98) {
            params.field_0x18 = 0;
            params.field_0x1c = 1;
        }
        params.field_0x30 = 1;
        __builtin_memset(params.field_0x20, 0, 16);
        fn_71006f4b0c(navigation, &params, 1);
    }
    if (self->owner)
        self->owner->field_0x13d2 = false;
    UnkInPathPatrolOwner_l38ba54* owner = self->owner;
    bool set = false;
    if (owner) {
        CBlackboard* blackboard = &owner->blackboard;
        if (ReadFlag_l38ba54(blackboard, g_unk_71726b7cb8,
                             base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x11e], &set) &&
            set) {
            const CStrId& name = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x11e];
            bool off = false;
            SetFlag_l38ba54(blackboard, g_unk_71726b7cb8, name, &off);
        }
    }
    if (UnkWeakHandle* handle = self->watched) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->link);
        ResetRef(&self->watched);
    }
}
