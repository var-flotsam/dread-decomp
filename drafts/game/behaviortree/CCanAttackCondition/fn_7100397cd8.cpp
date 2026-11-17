// behaviortree::CCanAttackCondition's unit (tools/tu_map.py): whether the actor can attack its target with
// the named attack (or the AI's current one, or one the AI picks), measuring the distance for the query.
// Names and layouts are placeholders.
#include "base/UnkVtableList.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU394.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include <math.h>

using base::math::CVector2D;
using base::math::CVector3D;

// A list of nodes (link at +8) freed through the allocator.
struct UnkCanAttackNodeList_ovb0137 : base::UnkVtableList<void> {
    ~UnkCanAttackNodeList_ovb0137() {
        vtable = g_unk_7101a21540 + 0x10;
        void* n = head;
        while (n) {
            void* next = *reinterpret_cast<void**>(static_cast<char*>(n) + 8);
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkAttackQuery_ovb0115;
struct UnkScorpiusAI_b69a4;
struct UnkAIAttackComponent4_ovb0050;
struct UnkGoliathXSlamAttack_b58a3;

// A counted reference released on destruction.
struct UnkCanAttackRef_ovb0137 {
    UnkRefObject* p;
    ~UnkCanAttackRef_ovb0137() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// The query an attack check takes (fn_71002c6f80 builds it from the actor and the parameters).
struct UnkAttackQuery_l397cd8 {
    UnkCanAttackRef_ovb0137 ref;
    f32 field_0x8;  // the distance to the target
    f32 field_0xc;
    u8 pad_0x10[0x9];
    u8 field_0x19;
    u8 pad_0x1a[0xe];
    UnkCanAttackNodeList_ovb0137 nodes;  // 0x28

    UnkAttackQuery_l397cd8(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

struct UnkAttack_l397cd8 {
    void** vtable;

    bool vf_0x58(UnkAttackQuery_l397cd8* query, u32 mode) {
        return reinterpret_cast<bool (*)(UnkAttack_l397cd8*, UnkAttackQuery_l397cd8*, u32)>(
            vtable[0x58 / 8])(this, query, mode);
    }
};

struct UnkAttackState_l397cd8 {
    u8 pad_0x0[0xec];
    s32 field_0xec;
};

struct UnkAI_l397cd8 {
    void** vtable;
    u8 pad_0x8[0x1138];
    u32 mode;  // 0x1140
    u8 pad_0x1144[0x4];
    UnkAttack_l397cd8* attack;  // 0x1148
    u8 pad_0x1150[0x18];
    u8 params[8];  // 0x1168, the attack parameters
    u8 pad_0x1170[0x4a0];
    UnkAttackState_l397cd8* field_0x1610;
    u8 pad_0x1618[0x13081];
    u8 field_0x14699;

    void vf_0x6b8(UnkAttackQuery_l397cd8* query) {
        reinterpret_cast<void (*)(UnkAI_l397cd8*, UnkAttackQuery_l397cd8*)>(vtable[0x6b8 / 8])(this, query);
    }
};
static_assert(__builtin_offsetof(UnkAI_l397cd8, field_0x1610) == 0x1610, "UnkAI_l397cd8 layout");
static_assert(__builtin_offsetof(UnkAI_l397cd8, field_0x14699) == 0x14699, "UnkAI_l397cd8 layout");

struct UnkCanAttackActor_l397cd8 {
    u8 pad_0x0[0x44];
    f32 field_0x44;  // translation x
    u8 pad_0x48[0xc];
    f32 field_0x54;  // translation y
    u8 pad_0x58[0xc];
    f32 field_0x64;  // translation z
    u8 pad_0x68[0x1b8];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkAI_l397cd8* ai;  // 0x240
    u8 pad_0x248[0x30];
    UnkAIAttackComponent4_ovb0050* attack_component;  // 0x278
};

// The condition's attack name (a string: its text first).
struct UnkCanAttackName_l397cd8 {
    const char* data;
};

struct UnkCanAttack_l397cd8 {
    u8 pad_0x0[0x18];
    UnkCanAttackActor_l397cd8* actor;  // 0x18
    u8 pad_0x20[0x18];
    UnkCanAttackName_l397cd8 name;  // 0x38
    u8 pad_0x40[0x28];
    bool field_0x68;
    bool field_0x69;  // the query gets no distance
    bool field_0x6a;  // with no attack, let the AI pick one
};

UnkAttack_l397cd8* fn_71010ffe3c(UnkAI_l397cd8* ai, UnkAttackQuery_l397cd8* query, u32 mode);
bool fn_7101134648(UnkAIAttackComponent4_ovb0050* self, UnkGoliathXSlamAttack_b58a3* attack, bool flag);

// Builds the query for the actor, fills in the condition's flag and, unless told not to, the distance
// to the target, lets the AI adjust it, and returns what `check` answers on it.
template <typename Check>
static inline auto Query_l397cd8(UnkCanAttack_l397cd8* self, UnkAI_l397cd8* ai, UnkActor* target, Check check) {
    UnkAttackQuery_l397cd8 query(reinterpret_cast<UnkActor*>(self->actor), ai->params);
    query.field_0x19 = self->field_0x68;
    if (!self->field_0x69) {
        f32 distance;
        // Along x alone when the AI's attack state says so, else in the plane.
        if (ai && ai->field_0x1610 && ai->field_0x1610->field_0xec == 1) {
            distance = __builtin_fabsf(self->actor->field_0x44 - target->field_0x44);
        } else {
            UnkCanAttackActor_l397cd8* actor = self->actor;
            CVector3D diff;
            diff.x = actor->field_0x44 - target->field_0x44;
            diff.y = actor->field_0x54 - target->field_0x54;
            diff.z = actor->field_0x64 - target->field_0x64;
            CVector2D flat;
            fn_710007b430(&flat, &diff);
            distance = sqrtf(flat.x * flat.x + flat.y * flat.y);
        }
        query.field_0x8 = distance;
        query.field_0xc = distance;
    }
    if (ai->field_0x14699)
        ai->vf_0x6b8(&query);
    return check(&query);
}

// Whether the actor can attack its target: with the attack named at +0x38 (or the AI's current one,
// or, when +0x6a allows it, one the AI picks for the query), the attack holds on a query carrying the
// distance to the target, the attack component can start it, and the actor is not turning (or is
// aborting a turn).
bool fn_7100397cd8(UnkCanAttack_l397cd8* self) {
    UnkActor* target = self->actor ? fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor)) : nullptr;
    UnkAI_l397cd8* ai = self->actor ? self->actor->ai : nullptr;
    UnkAnimationComponent* animation = self->actor ? self->actor->animation : nullptr;
    UnkAttack_l397cd8* attack;
    if (self->name.data && self->name.data[0])
        attack = ai ? static_cast<UnkAttack_l397cd8*>(fn_7101109df4(reinterpret_cast<UnkScorpiusAI_b69a4*>(ai),
                                                                    &self->name))
                    : nullptr;
    else
        attack = ai ? ai->attack : nullptr;
    if (!target)
        return false;
    bool ok;
    if (attack) {
        ok = Query_l397cd8(self, ai, target, [attack, ai](UnkAttackQuery_l397cd8* query) {
            return attack->vf_0x58(query, ai->mode);
        });
    } else {
        if (!self->field_0x6a)
            return false;
        attack = Query_l397cd8(self, ai, target, [ai](UnkAttackQuery_l397cd8* query) {
            return fn_71010ffe3c(ai, query, 0);
        });
        ok = attack != nullptr;
    }
    if (!ok || !self->actor)
        return false;
    UnkAIAttackComponent4_ovb0050* component = self->actor->attack_component;
    if (!component)
        return false;
    if (!fn_7101134648(component, reinterpret_cast<UnkGoliathXSlamAttack_b58a3*>(attack), false))
        return false;
    if (!fn_710117b348(animation, fn_7100d2b3a8()))
        return true;
    return fn_710117b348(self->actor->animation, fn_7100bfac38());
}
