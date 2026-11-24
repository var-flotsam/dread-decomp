// CNailongAIComponent unit (tools/tu_map.py unit 937): the AI's update. After the behaviour tree's step,
// eases the "UpDownFactor" towards the owner's vertical heading, then, when the owner can attack, runs the
// AI's attacks with a query built from the owner and the attack parameters. Names and layouts are
// placeholders.
#include <math.h>
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "base/UnkVtableList.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/CAttackComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTU394.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;

// A list of nodes (link at +8) freed through the allocator.
struct UnkAttackNodeList_l6212a8 : base::UnkVtableList<void> {
    ~UnkAttackNodeList_l6212a8() {
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

// A counted reference released on destruction.
struct UnkAttackRef_l6212a8 {
    UnkRefObject* p;
    ~UnkAttackRef_l6212a8() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkAttackQuery_l6212a8 {
    UnkAttackRef_l6212a8 ref;
    f32 distance;    // 0x8
    f32 distance2;   // 0xc
    u8 pad_0x10[0x18];
    UnkAttackNodeList_l6212a8 nodes;  // 0x28

    UnkAttackQuery_l6212a8(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

struct UnkNailongAttackSettings_l6212a8 {
    u8 pad_0x0[0xec];
    bool field_0xec;
};

struct UnkNailongAttack_l6212a8 {
    void** vtable;
    u8 pad_0x8[0x18];
    UnkNailongAttackSettings_l6212a8* settings;  // 0x20
    u8 pad_0x28[0x70];

    // vtable slot 0x58: whether the attack starts for the query.
    bool vf_0x58(UnkAttackQuery_l6212a8* query, u32 arg) {
        return reinterpret_cast<bool (*)(UnkNailongAttack_l6212a8*, UnkAttackQuery_l6212a8*, u32)>(
            vtable[0x58 / 8])(this, query, arg);
    }
};

struct UnkNailongMovement_l6212a8 {
    u8 pad_0x0[0x40];
    f32 x;  // 0x40
    f32 y;  // 0x44
    f32 z;  // 0x48
    u8 pad_0x4c[0x30];
    f32 field_0x7c;
};

struct UnkNailongBagOwner_l6212a8 {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag bag;  // 0x80
};

struct UnkNailongAI_l6212a8;

struct UnkNailongOwner_l6212a8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1f0 - 0x78];
    UnkNailongMovement_l6212a8* movement;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkNailongBagOwner_l6212a8* bag_owner;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkNailongAI_l6212a8* ai;  // 0x240
    u8 pad_0x248[0x278 - 0x248];
    void* attack_component;  // 0x278
};
static_assert(__builtin_offsetof(UnkNailongOwner_l6212a8, attack_component) == 0x278, "layout");

struct UnkNailongAI_l6212a8 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkNailongOwner_l6212a8* owner;  // 0x30
    u8 pad_0x38[0x1140 - 0x38];
    u32 field_0x1140;
    u8 pad_0x1144[0x1168 - 0x1144];
    u8 params[8];  // 0x1168, the attack parameters
    u8 pad_0x1170[0x15d0 - 0x1170];
    bool field_0x15d0;
    u8 pad_0x15d1[0x146c8 - 0x15d1];
    UnkNailongAttack_l6212a8 attack;   // 0x146c8
    UnkNailongAttack_l6212a8 attack2;  // 0x14760
    u8 pad_0x147f8[0x14838 - 0x147f8];
    f32 up_down;  // 0x14838
    u8 pad_0x1483c[4];
    f32 timer;  // 0x14840
    u8 pad_0x14844[0x148c0 - 0x14844];
    f32 threshold;  // 0x148c0
    bool pending;   // 0x148c4
    s32 mode;       // 0x148c8
};
static_assert(__builtin_offsetof(UnkNailongAI_l6212a8, mode) == 0x148c8, "layout");

extern CType g_unk_71726f7e68 __attribute__((visibility("hidden")));  // CNailongAIComponent's type

void CNailongAIComponent_FlipDirectionToOwnerTransform(UnkNailongAI_l6212a8* self);
void fn_710110592c(UnkNailongAI_l6212a8* ai, UnkNailongAttack_l6212a8* attack);

// The object when it is a CNailongAIComponent, else null (the getter runs first so the type is registered).
static inline UnkNailongAI_l6212a8* AsNailongAI(UnkNailongAI_l6212a8* object) {
    if (!object)
        return nullptr;
    const CType* type = object->vf_0x0();
    fn_710061f464();
    return fn_710009d50c(type, &g_unk_71726f7e68) ? object : nullptr;
}

void fn_71006212a8(UnkNailongAI_l6212a8* self, f32 dt) {
    fn_7100369c44(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    if (self->field_0x15d0)
        return;

    CNailongAIComponent_FlipDirectionToOwnerTransform(self);
    UnkNailongMovement_l6212a8* movement = self->owner->movement;
    f32 x = movement->x;
    f32 y = movement->y;
    f32 z = movement->z;
    f32 length = sqrtf(x * x + y * y + z * z);
    f32 heading = y * (1.0f / (length + 0.0001f));
    fn_710002614c(&self->owner->transform);
    UnkNailongOwner_l6212a8* owner = self->owner;
    f32 target = heading * owner->movement->field_0x7c;
    f32 step = dt + dt;
    f32 current = self->up_down;
    f32 low = current - step;
    f32 high = step + current;
    f32 down = target > low ? target : low;
    f32 up = target < high ? target : high;
    self->up_down = !(target > current) ? down : up;
    unk_prop_bag::fn_710033187c(&owner->bag_owner->bag, CStrId("UpDownFactor", true), self->up_down);

    UnkNailongAI_l6212a8* ai = AsNailongAI(self->owner->ai);
    UnkNailongAttack_l6212a8* attack = &ai->attack;
    if (self->mode == 0 || self->mode == 2) {
        void* component = self->owner->attack_component;
        if (!component)
            return;
        UnkNailongAttackSettings_l6212a8* settings = attack->settings;
        if (fn_7101136308(component))
            return;
        UnkAttackQuery_l6212a8 query(reinterpret_cast<UnkActor*>(self->owner), ai->params);
        if (!settings->field_0xec || (self->timer += dt) >= self->threshold) {
            if (attack->vf_0x58(&query, ai->field_0x1140)) {
                if (settings->field_0xec)
                    fn_710110592c(ai, attack);
                else
                    self->pending = true;
            }
        }
        if (self->pending) {
            self->pending = false;
            fn_710110592c(ai, attack);
        }
    } else {
        void* component = self->owner->attack_component;
        if (!component || fn_7101136308(component))
            return;
        UnkAttackQuery_l6212a8 query(reinterpret_cast<UnkActor*>(self->owner), ai->params);
        if ((self->timer += dt) >= self->threshold) {
            UnkNailongAttack_l6212a8* attack2 = &ai->attack2;
            if (attack2->vf_0x58(&query, ai->field_0x1140))
                fn_710110592c(ai, attack2);
        }
    }
}
