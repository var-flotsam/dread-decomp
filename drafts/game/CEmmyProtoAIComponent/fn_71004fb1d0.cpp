// CEmmyProtoAIComponent's unit (tools/tu_map.py): whether the EMMY can reach its target now. The target's
// movement must not be in the blocking state and must hold an entry of kind 3; the EMMY must be ready
// (its key list at +0x29418, or the flag at +0x29f26), facing the target's side and close enough that the
// entry's reach covers the gap between the two shapes. Then it marks the grab, plays track 0x10 and, when
// vf_0x818 allows, adds the blackboard key to both key lists. Names and layouts are placeholders.
#include <algorithm>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU39.h"
#include "types.h"

__attribute__((visibility("hidden"))) extern base::global::CStrId g_unk_71726dd1f8;

// A shape with a box that is brought up to date on demand.
struct UnkShape_l4fb1d0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void Update();  // 0x98
    u8 pad_0x8[0x38];
    f32 min_x;  // 0x40
    u8 pad_0x44[4];
    f32 max_x;  // 0x48
    u8 pad_0x4c[0x14];
    bool dirty;  // 0x60

    f32 Width() {
        if (dirty)
            Update();
        return max_x - min_x;
    }
};

struct UnkTarget_l4fb1d0 {
    u8 pad_0x0[0x10];
    s32 kind;
    u8 pad_0x14[0x1f8 - 0x14];
    UnkGuiKeyList keys;  // 0x1f8
};

f32 fn_7100996dcc(UnkTarget_l4fb1d0* target, u32 index);

#pragma pack(push, 4)
struct UnkTargetList_l4fb1d0 {
    u32 count;
    UnkTarget_l4fb1d0* items[1];

    UnkTarget_l4fb1d0** begin() { return items; }
    UnkTarget_l4fb1d0** end() { return items + count; }

    UnkTarget_l4fb1d0* FindKind(s32 kind) {
        UnkTarget_l4fb1d0** it =
            std::find_if(begin(), end(), [kind](UnkTarget_l4fb1d0* entry) { return entry->kind == kind; });
        return it != end() ? *it : nullptr;
    }
};
#pragma pack(pop)

struct UnkMovement_l4fb1d0 {
    void** vtable;
    u8 pad_0x8[0x2d18 - 0x8];
    UnkTargetList_l4fb1d0 targets;  // 0x2d18

    UnkShape_l4fb1d0* vf_0x2a8() {
        return reinterpret_cast<UnkShape_l4fb1d0* (*)(UnkMovement_l4fb1d0*)>(vtable[0x2a8 / 8])(this);
    }
};

// The actor's slots this function reads past what game/UnkActor.h spells out.
struct UnkActorShapes_l4fb1d0 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* shapes;  // 0x2a0
};

struct UnkEmmyProtoAI_l4fb1d0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x29418 - 0x38];
    UnkGuiKeyList keys;  // 0x29418
    u8 pad_0x29448[0x29f26 - 0x29448];
    bool ready;  // 0x29f26
    u8 pad_0x29f27[0x2a709 - 0x29f27];
    bool grabbed;  // 0x2a709
    u8 pad_0x2a70a;
    bool field_0x2a70b;

    bool vf_0x818() { return reinterpret_cast<bool (*)(UnkEmmyProtoAI_l4fb1d0*)>(vtable[0x818 / 8])(this); }
};

static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

static inline UnkMovement_l4fb1d0* Movement(UnkActor* actor) {
    return static_cast<UnkMovement_l4fb1d0*>(actor->field_0x208);
}

bool fn_71004fb1d0(UnkEmmyProtoAI_l4fb1d0* self) {
    if (fn_71010d1598(static_cast<UnkGroundPlayer_b52*>(fn_7100d7c990(self->owner)->field_0x208)))
        return false;

    UnkTarget_l4fb1d0* target = Movement(fn_7100d7c990(self->owner))->targets.FindKind(3);
    if (!target)
        return false;

    UnkActor* other = fn_7100d7c990(self->owner);
    bool ready = fn_710002fb70(&self->keys) || self->ready;
    if (!other || !ready)
        return false;

    f32 own_x = self->owner->field_0x44;
    f32 other_x = other->field_0x44;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(other->field_0x38));
    f32 facing = m.m[2];
    if (!((facing > 0.0f && own_x > other_x) || (facing < 0.0f && own_x < other_x)))
        return false;

    auto* own_shape = reinterpret_cast<UnkShape_l4fb1d0*>(
        fn_71008640a4(reinterpret_cast<UnkActorShapes_l4fb1d0*>(self->owner)->shapes, &g_unk_71726dd1f8));
    UnkShape_l4fb1d0* other_shape = Movement(other)->vf_0x2a8();
    if (!own_shape || !other_shape)
        return false;

    f32 reach = fn_7100996dcc(target, 0) + 42.0f;
    f32 gap = __builtin_fabsf(own_x - other_x) - own_shape->Width() * 0.5f - other_shape->Width() * 0.5f - 5.0f;
    if (!(reach >= gap))
        return false;

    self->grabbed = true;
    self->field_0x2a70b = false;
    fn_710117662c(static_cast<UnkActorTarget*>(fn_7100d7c990(self->owner)->field_0x220), 0x10, 1000, true, false,
                  -1.0f);
    if (self->vf_0x818() && fn_7100d7c990(self->owner)->field_0x208) {
        fn_710002fd10(&target->keys, const_cast<base::global::CStrId*>(&GetQualifiedId(g_unk_71726dd158).id));
        fn_710002fd10(&self->keys, const_cast<base::global::CStrId*>(&GetQualifiedId(g_unk_71726dd158).id));
    }
    return true;
}
