// Unit 1102 (data/tu_map.csv, hint CXParasiteAIComponent, no reliable name): parts of the X parasite
// component. fn_71006e6d10 resets the state block (and the slot table's "interior" request), fn_71006e808c
// hands the grab link over to the entry fn_71006eb7cc picks (the farthest one), fn_71006e8208 starts or stops the
// state's effect, fn_71006e8a18 steps the state, fn_71006e9fcc takes a target. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CActorComponent.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkModelManager.h"
#include "game/UnkSlots.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU1102.h"
#include "game/UnkXParasiteResult.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "game/UnkXParasiteAI.h"


// ---- fn_71006e808c ----
struct UnkActor;

void fn_7101127c24(void* obj, UnkActor* owner, bool, bool b, s32 c, bool d, void* e);

static inline void* Inner(UnkWeakHandle* p) { return p ? p->target : nullptr; }

void fn_71006e808c(UnkXParasiteAI* self) {
    if (!self->field_0x147b0 || !self->field_0x147b0->target)
        return;
    UnkXParasiteResult_b0252 found = fn_71006eb7cc(self);
    if (found.ptr && found.ptr->target && self->field_0x147b8) {
        fn_7101127c24(Inner(self->field_0x147b0), self->owner, 0, false, 5, false, nullptr);
        fn_71006eb8cc(reinterpret_cast<base::UnkRefHandle*>(&self->field_0x147b0), reinterpret_cast<const base::UnkRefHandle*>(&found));
        self->field_0x147b8 = false;
    }
    fn_7101127c24(Inner(self->field_0x147b0), self->owner, 1, self->field_0x147b8 ? false : self->released == 0,
                  self->field_0x14724, self->field_0x146e8 != 0, Inner(found.ptr));
    reinterpret_cast<base::UnkRefHandle*>(&self->field_0x147b0)->Set(nullptr);
}

// ---- fn_71006e9fcc ----
struct UnkObj_7101129240;
// The other declarations of this symbol drop the result; it is the actor the object stands for.

static inline UnkActor* PickActor(UnkObj_7101129240* object, bool primary) {
    return reinterpret_cast<UnkActor* (*)(UnkObj_7101129240*, bool)>(fn_7101129b08)(object, primary);
}

void fn_710110e3dc(void* component);

struct UnkXParasiteHandleOwner_9fcc {
    u8 pad_0x0[0x10];
    UnkObj_7101129240* field_0x10;
};

void fn_71006e9fcc(UnkXParasiteAI* self, UnkActor* target) {
    if (self->link && self->link->target)
        return;
    if (!target) {
        UnkXParasiteResult_b0252 found = fn_71006eb7cc(self);
        if (!found.ptr)
            return;
        target = found.ptr->target ? PickActor(reinterpret_cast<UnkObj_7101129240*>(found.ptr->target), true) : nullptr;
    }
    if (!target)
        return;
    fn_710110e3dc(target->field_0x240);
    AssignRef(&self->link, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(target)));
    fn_71006ea824(self, target);
    for (u32 i = 0; i < self->entries.count; ++i) {
        UnkRefObject* p = self->entries.data[i];
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
    self->entries.count = 0;
}

// ---- fn_71006e8a18 ----
struct UnkStruct_710024fa0c;
struct UnkXParasiteMode_A3_b75;

void fn_71006e75e8(UnkXParasiteAI* self, bool on);

static inline bool Moving(UnkXParasiteAI* s) {
    return s->field_0x14714 - 0.001f > 0.0f || s->field_0x14714 + 0.001f < 0.0f || s->field_0x14718 - 0.001f > 0.0f ||
           s->field_0x14718 + 0.001f < 0.0f || s->field_0x1471c - 0.001f > 0.0f || s->field_0x1471c + 0.001f < 0.0f;
}

void fn_71006e8a18(UnkXParasiteAI* self) {
    if (self->state == 0) {
        if (self->owner->field_0x218)
            fn_710024fa0c(static_cast<UnkStruct_710024fa0c*>(self->owner->field_0x218), false);
    }
    if (self->state == 5) {
        self->field_0x146f0 = 0;
        self->field_0x146f8 = 0;
    }
    if (self->field_0x14720 && Moving(self)) {
        if (self->link && self->link->target) {
            UnkXParasiteGrabbed* object = Grabbed(self->link);
            if (object->partner_link && object->partner_link->partner) {
                UnkXParasitePartner* partner = object->partner_link->partner;
                typedef u32 __attribute__((may_alias)) AliasU32;
                *reinterpret_cast<AliasU32*>(&partner->field_0x2bc) = partner->field_0x2c0;
                object = Grabbed(self->link);
            }
            if (object->listener)
                object->listener->Notify(self);
            if (UnkWeakHandle* l = self->link) {
                if (--l->ref_count == 0)
                    l->vf_0x10();
                self->link = nullptr;
            }
            self->released = true;
        } else {
            fn_71006e892c(self);
        }
        fn_7100232c68(self->owner);
    } else {
        fn_71006e75e8(self, true);
    }
    fn_71006e9384(self, reinterpret_cast<const UnkXParasiteMode_A3_b75*>(&self->field_0x146e4));
    self->field_0x146e4 = 0x7fffffff;
}

// ---- fn_71006e8208 ----
struct UnkWorld;
struct UnkPart;
struct UnkRinkaControl_ruc;
struct UnkRinkaControl_ovb0090;
struct UnkRinkaEntry_ruc;
struct UnkFxObject_ovb0041;

struct UnkXParasiteKeyed_8208 {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};
extern UnkXParasiteKeyed_8208* g_unk_717270b910[2] __attribute__((visibility("hidden")));

struct UnkXParasiteActor_8208 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x280 - 0x94];
    void* control;  // 0x280
};

struct UnkXParasitePartHandle_8208 {
    u8 pad_0x0[0xb];
    u8 field_0xb;
};

struct UnkXParasiteVec3_8208 {
    f32 x, y, z;
};

static inline UnkXParasiteActor_8208* Actor(UnkXParasiteAI* self) { return reinterpret_cast<UnkXParasiteActor_8208*>(self->owner); }

void fn_71006e8208(UnkXParasiteAI* self, bool on) {
    if (self->field_0x14724 == 4)
        return;
    void* control = Actor(self)->control;
    if (!control)
        return;
    if (on) {
        if (!g_unk_71726b0cd8) {
            UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
            fn_71011a870c(m);
            g_unk_71726b0cd8 = m;
        }
        UnkModelManager* manager = g_unk_71726b0cd8;
        UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, CurrentBinding(self)->effect, false, true);
        if (!fx)
            return;
        UnkVec3f position = Actor(self)->xform.GetTranslation();
        fn_7100146d84(fx, &position);
        fn_7101152988(static_cast<UnkRinkaControl_ovb0090*>(control), fx, reinterpret_cast<UnkRinkaEntry_ruc**>(g_unk_717270b910),
                      nullptr, 0);
        const UnkVec3f& src = *reinterpret_cast<const UnkVec3f*>(&g_unk_7101647790);
        UnkVec3f first(src);
        UnkVec3f second(src);
        fn_710115217c(control, fx, 0, 7, &g_unk_7101d470e0, reinterpret_cast<const base::math::CVector3D&>(first),
                      reinterpret_cast<const base::math::CVector3D&>(second), nullptr);
        fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(fx), true);
    } else {
        UnkPart* part = fn_7101152b90(static_cast<UnkWorld*>(control), g_unk_717270b910[0]->key);
        if (!part)
            return;
        if (!fn_710114e668(reinterpret_cast<UnkObject*>(control),
                           reinterpret_cast<UnkXParasitePartHandle_8208*>(part)->field_0xb))
            return;
        fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(part), false);
        fn_710115596c(static_cast<UnkRinkaControl_ruc*>(control), part);
    }
}

void fn_71006e6d10(UnkXParasiteAI* self) {
    fn_71003695b8(reinterpret_cast<UnkBehaviorTreeAI*>(self));
    ResetRef(&self->field_0x146d0);
    self->field_0x14724 = 5;
    self->field_0x146ea = false;
    self->field_0x146e4 = 0x7fffffff;
    self->state = 1;
    self->field_0x146e8 = true;
    self->field_0x146eb = false;
    self->field_0x146e9 = false;
    self->field_0x146fc = 0;
    ResetRef(&self->link);
    ResetRef(&self->field_0x14798);
    self->field_0x147a8 = 0.0f;
    for (u32 i = 0; i < self->entries.count; ++i) {
        UnkRefObject* p = self->entries.data[i];
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
    self->entries.count = 0;
    self->field_0x1470c = -1.0f;
    self->field_0x14714 = 0.0f;
    self->field_0x146f0 = 0;
    self->field_0x14720 = false;
    self->field_0x146f8 = 0;
    self->field_0x14718 = 0.0f;
    self->field_0x1471c = 0.0f;
    self->field_0x14700 = 0;
    self->field_0x14710 = 0;
    self->field_0x14708 = 0;
    ResetRef(&self->field_0x147b0);
    self->field_0x147b8 = false;
    self->released = false;
    self->field_0x146ec = false;
    if (UnkXParasiteHolder* holder = ModelHolder(self->owner)) {
        if (UnkXParasiteShape* shape = holder->shape) {
            u64 key = fn_7100001570("interior", 8);
            base::color::CColor4F color = {};
            fn_71001c28f8(shape->slots, key, 3, &color, 1);
        }
    }
}
