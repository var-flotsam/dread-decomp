// Unit 1102 (data/tu_map.csv, hint CXParasiteAIComponent, no reliable name): parts of the X parasite
// component. fn_71006e6d10 resets the state block (and the slot table's "interior" request), fn_71006e808c
// hands the grab link over to the nearest target found by fn_71006eb7cc, fn_71006e8208 starts or stops the
// state's effect, fn_71006e8a18 steps the state, fn_71006e9fcc takes a target. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CActorComponent.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkModelManager.h"
#include "game/UnkSlots.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU1102.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"


// The X parasite component and the reference-counted result of the nearest-target search (fn_71006eb7cc,
// not matched yet; the callers share this declaration).
struct UnkXParasiteAI_b0252;

struct UnkXParasiteFound_b0252 : UnkRefObject {
    void* field_0x10;
};

struct UnkXParasiteResult_b0252 {
    UnkXParasiteFound_b0252* ptr;
    ~UnkXParasiteResult_b0252() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

UnkXParasiteResult_b0252 fn_71006eb7cc(UnkXParasiteAI_b0252* self);

// ---- fn_71006e808c ----
struct UnkActor;

struct UnkXParasiteLock_808c {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x146e8 - 0x38];
    u8 field_0x146e8;
    u8 pad_0x146e9[0x146e8 + 0x3c - 0x146e9];
    s32 field_0x14724;
    u8 pad_0x14728[0x147b0 - 0x14728];
    UnkXParasiteFound_b0252* handle;  // 0x147b0
    bool field_0x147b8;
    bool field_0x147b9;
};
static_assert(__builtin_offsetof(UnkXParasiteLock_808c, handle) == 0x147b0, "placeholder layout");

base::UnkRefHandle* fn_71006eb8cc(base::UnkRefHandle*, const base::UnkRefHandle*);
void fn_7101127c24(void* obj, UnkActor* owner, bool, bool b, s32 c, bool d, void* e);

static inline void* Inner(UnkXParasiteFound_b0252* p) { return p ? p->field_0x10 : nullptr; }

void fn_71006e808c(UnkXParasiteLock_808c* self) {
    if (!self->handle || !self->handle->field_0x10)
        return;
    UnkXParasiteResult_b0252 found = fn_71006eb7cc(reinterpret_cast<UnkXParasiteAI_b0252*>(self));
    if (found.ptr && found.ptr->field_0x10 && self->field_0x147b8) {
        fn_7101127c24(Inner(self->handle), self->owner, 0, false, 5, false, nullptr);
        fn_71006eb8cc(reinterpret_cast<base::UnkRefHandle*>(&self->handle), reinterpret_cast<const base::UnkRefHandle*>(&found));
        self->field_0x147b8 = false;
    }
    fn_7101127c24(Inner(self->handle), self->owner, 1, self->field_0x147b8 ? false : self->field_0x147b9 == 0, self->field_0x14724,
                  self->field_0x146e8 != 0, Inner(found.ptr));
    reinterpret_cast<base::UnkRefHandle*>(&self->handle)->Set(nullptr);
}

// ---- fn_71006e9fcc ----
struct UnkObj_7101129240;
// The other declarations of this symbol drop the result; it is the actor the object stands for.

static inline UnkActor* PickActor(UnkObj_7101129240* object, bool primary) {
    return reinterpret_cast<UnkActor* (*)(UnkObj_7101129240*, bool)>(fn_7101129b08)(object, primary);
}

struct UnkXParasiteTargets_9fcc {
    UnkRefObject** data;  // 0x14740
    u32 count;
    u8 pad_0xc[0x44];
    UnkWeakHandle* handle;  // 0x14790
};

struct UnkXParasiteSelect_9fcc {
    u8 pad_0x0[0x14740];
    UnkXParasiteTargets_9fcc targets;
};
static_assert(__builtin_offsetof(UnkXParasiteSelect_9fcc, targets.handle) == 0x14790, "placeholder layout");

void fn_710110e3dc(void* component);

struct UnkXParasiteHandleOwner_9fcc {
    u8 pad_0x0[0x10];
    UnkObj_7101129240* field_0x10;
};

void fn_71006e9fcc(UnkXParasiteSelect_9fcc* self, UnkActor* target) {
    if (self->targets.handle && self->targets.handle->target)
        return;
    if (!target) {
        UnkXParasiteResult_b0252 found = fn_71006eb7cc(reinterpret_cast<UnkXParasiteAI_b0252*>(self));
        if (!found.ptr)
            return;
        target = found.ptr->field_0x10 ? PickActor(static_cast<UnkObj_7101129240*>(found.ptr->field_0x10), true) : nullptr;
    }
    if (!target)
        return;
    fn_710110e3dc(target->field_0x240);
    AssignRef(&self->targets.handle, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(target)));
    fn_71006ea824(self, target);
    for (u32 i = 0; i < self->targets.count; ++i) {
        UnkRefObject* p = self->targets.data[i];
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
    self->targets.count = 0;
}

// ---- fn_71006e8a18 ----
struct UnkStruct_710024fa0c;
struct UnkXParasiteGrab_xpa;
struct UnkXParasiteAI_A3_b75;
struct UnkXParasiteMode_A3_b75;

struct UnkXParasiteSelf_8a18;
void fn_71006e75e8(UnkXParasiteSelf_8a18* self, bool on);

struct UnkXParasitePartner_8a18 {
    u8 pad_0x0[0x2bc];
    u32 field_0x2bc;
    u32 field_0x2c0;
};

struct UnkXParasitePartnerLink_8a18 {
    u8 pad_0x0[0x10];
    UnkXParasitePartner_8a18* partner;  // 0x10
};

struct UnkXParasiteListener_8a18 {
    void** vtable;
    void Notify(void* self) { reinterpret_cast<void (*)(UnkXParasiteListener_8a18*, void*)>(vtable[0x410 / 8])(this, self); }
};

struct UnkXParasiteGrabbed_8a18 {
    u8 pad_0x0[0x240];
    UnkXParasiteListener_8a18* listener;  // 0x240
    u8 pad_0x248[0x150];
    UnkXParasitePartnerLink_8a18* partner_link;  // 0x398
};

struct UnkXParasiteLink_8a18 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    UnkXParasiteGrabbed_8a18* object;  // 0x10
};

struct UnkXParasiteOwner_8a18 {
    u8 pad_0x0[0x218];
    UnkStruct_710024fa0c* field_0x218;
};

struct UnkXParasiteState_8a18 {
    s32 state;  // 0x146e0
    s32 field_0x4;
    u8 pad_0x8[8];
    u64 field_0x10;
    u32 field_0x18;
    u8 pad_0x1c[0x18];
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    u8 pad_0x41[0x6f];
    UnkXParasiteLink_8a18* link;  // 0xb0
    u8 pad_0xb8[0x21];
    bool released;  // 0xd9
};
static_assert(__builtin_offsetof(UnkXParasiteState_8a18, link) == 0xb0, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteState_8a18, released) == 0xd9, "placeholder layout");

struct UnkXParasiteSelf_8a18 {
    u8 pad_0x0[0x30];
    UnkXParasiteOwner_8a18* owner;  // 0x30
    u8 pad_0x38[0x146e0 - 0x38];
    UnkXParasiteState_8a18 s;  // 0x146e0
};

static inline bool Moving(UnkXParasiteState_8a18* s) {
    return s->field_0x34 - 0.001f > 0.0f || s->field_0x34 + 0.001f < 0.0f || s->field_0x38 - 0.001f > 0.0f ||
           s->field_0x38 + 0.001f < 0.0f || s->field_0x3c - 0.001f > 0.0f || s->field_0x3c + 0.001f < 0.0f;
}

void fn_71006e8a18(UnkXParasiteSelf_8a18* self) {
    UnkXParasiteState_8a18* s = &self->s;
    if (s->state == 0) {
        if (self->owner->field_0x218)
            fn_710024fa0c(self->owner->field_0x218, false);
    }
    if (s->state == 5) {
        s->field_0x10 = 0;
        s->field_0x18 = 0;
    }
    if (s->field_0x40 && Moving(s)) {
        if (s->link && s->link->object) {
            UnkXParasiteGrabbed_8a18* object = s->link->object;
            if (object->partner_link && object->partner_link->partner) {
                UnkXParasitePartner_8a18* partner = object->partner_link->partner;
                typedef u32 __attribute__((may_alias)) AliasU32;
                *reinterpret_cast<AliasU32*>(&partner->field_0x2bc) = partner->field_0x2c0;
                object = s->link->object;
            }
            if (object->listener)
                object->listener->Notify(self);
            if (UnkXParasiteLink_8a18* l = s->link) {
                if (--l->ref_count == 0)
                    l->Release();
                s->link = nullptr;
            }
            s->released = true;
        } else {
            fn_71006e892c(reinterpret_cast<UnkXParasiteGrab_xpa*>(self));
        }
        fn_7100232c68(self->owner);
    } else {
        fn_71006e75e8(self, true);
    }
    fn_71006e9384(reinterpret_cast<UnkXParasiteAI_A3_b75*>(self), reinterpret_cast<const UnkXParasiteMode_A3_b75*>(&s->field_0x4));
    s->field_0x4 = 0x7fffffff;
}

// ---- fn_71006e8208 ----
struct UnkWorld;
struct UnkPart;
struct UnkRinkaControl_ruc;
struct UnkRinkaControl_ovb0090;
struct UnkRinkaEntry_ruc;
struct UnkFxObject_ovb0041;

UnkPart* fn_7101152b90(UnkWorld*, u64);
bool fn_710114e668(UnkWorld*, u8);
void fn_7100146d84(UnkFxObject_ovb0041*, const UnkVec3f*);
void fn_710115217c(void*, void*, u32, u32, const base::global::CStrId*, const base::math::CVector3D&,
                   const base::math::CVector3D&, void*);

struct UnkXParasiteKeyed_8208 {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};
extern UnkXParasiteKeyed_8208* g_unk_717270b910[2] __attribute__((visibility("hidden")));
extern char g_unk_7101647790[];

struct UnkXParasiteBinding_8208 {
    u8 pad_0x0[0x10];
    u8 effect[0x28];  // 0x10
};
static_assert(sizeof(UnkXParasiteBinding_8208) == 0x38, "placeholder layout");

struct UnkXParasiteBindings_8208 {
    UnkXParasiteBinding_8208* values;
    u32* keys;
    u32 count;
};

struct UnkXParasiteActor_8208 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x280 - 0x94];
    void* control;  // 0x280
};

struct UnkXParasiteSelf_8208 {
    u8 pad_0x0[0x30];
    UnkXParasiteActor_8208* owner;  // 0x30
    u8 pad_0x38[0x14724 - 0x38];
    s32 state;  // 0x14724
    u8 pad_0x14728[0x14768 - 0x14728];
    UnkXParasiteBindings_8208 bindings;  // 0x14768
};

struct UnkXParasitePartHandle_8208 {
    u8 pad_0x0[0xb];
    u8 field_0xb;
};

struct UnkXParasiteVec3_8208 {
    f32 x, y, z;
};

static inline UnkXParasiteBinding_8208* Find(UnkXParasiteSelf_8208* self) {
    u64 i = 0;
    for (; i < self->bindings.count; ++i) {
        if (self->bindings.keys[i] == (u32)self->state)
            break;
    }
    return &self->bindings.values[i];
}

void fn_71006e8208(UnkXParasiteSelf_8208* self, bool on) {
    if (self->state == 4)
        return;
    void* control = self->owner->control;
    if (!control)
        return;
    if (on) {
        if (!g_unk_71726b0cd8) {
            UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
            fn_71011a870c(m);
            g_unk_71726b0cd8 = m;
        }
        UnkModelManager* manager = g_unk_71726b0cd8;
        UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, Find(self)->effect, false, true);
        if (!fx)
            return;
        UnkVec3f position = self->owner->xform.GetTranslation();
        fn_7100146d84(fx, &position);
        fn_7101152988(static_cast<UnkRinkaControl_ovb0090*>(control), fx, reinterpret_cast<UnkRinkaEntry_ruc**>(g_unk_717270b910),
                      nullptr, 0);
        const UnkVec3f& src = *reinterpret_cast<const UnkVec3f*>(g_unk_7101647790);
        UnkVec3f first(src);
        UnkVec3f second(src);
        fn_710115217c(control, fx, 0, 7, &g_unk_7101d470e0, reinterpret_cast<const base::math::CVector3D&>(first),
                      reinterpret_cast<const base::math::CVector3D&>(second), nullptr);
        fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(fx), true);
    } else {
        UnkPart* part = fn_7101152b90(static_cast<UnkWorld*>(control), g_unk_717270b910[0]->key);
        if (!part)
            return;
        if (!fn_710114e668(static_cast<UnkWorld*>(control), reinterpret_cast<UnkXParasitePartHandle_8208*>(part)->field_0xb))
            return;
        fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(part), false);
        fn_710115596c(static_cast<UnkRinkaControl_ruc*>(control), part);
    }
}

// ---- fn_71006e6d10 ----
struct UnkXParasiteShape_6d10 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;  // 0x138
};

struct UnkXParasiteHolder_6d10 {
    u8 pad_0x0[0x50];
    UnkXParasiteShape_6d10* shape;  // 0x50
};

struct UnkXParasiteOwner_6d10 {
    u8 pad_0x0[0x228];
    UnkXParasiteHolder_6d10* holder;  // 0x228
};

// The reset-able state block at +0x146d0.
struct UnkXParasiteState_6d10 {
    UnkRefObject* field_0x0;
    u8 pad_0x8[8];
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u64 field_0x30;
    u32 field_0x38;
    f32 field_0x3c;
    u8 pad_0x40[0x14];
    s32 field_0x54;
    u8 pad_0x58[0x18];
    UnkRefObject** field_0x70;
    u32 field_0x78;
    u8 pad_0x7c[0x44];
    UnkRefObject* field_0xc0;
    UnkRefObject* field_0xc8;
    u8 pad_0xd0[8];
    u32 field_0xd8;
    u8 pad_0xdc[4];
    UnkRefObject* field_0xe0;
    u16 field_0xe8;
};
static_assert(__builtin_offsetof(UnkXParasiteState_6d10, field_0xe8) == 0xe8, "placeholder layout");

struct UnkXParasiteTail_6d10 {
    u64 a, b;
    u8 c;
};

struct UnkXParasiteSelf_6d10 {
    u8 pad_0x0[0x30];
    UnkXParasiteOwner_6d10* owner;  // 0x30
    u8 pad_0x38[0x146d0 - 0x38];
    UnkXParasiteState_6d10 s;  // 0x146d0
};

void fn_71006e6d10(UnkXParasiteSelf_6d10* self) {
    UnkXParasiteState_6d10* s = &self->s;
    fn_71003695b8(reinterpret_cast<UnkBehaviorTreeAI*>(self));
    ResetRef(&s->field_0x0);
    s->field_0x54 = 5;
    s->field_0x10 = 1;
    s->field_0x14 = 0x7fffffff;
    s->field_0x18 = 1;
    s->field_0x2c = 0;
    ResetRef(&s->field_0xc0);
    ResetRef(&s->field_0xc8);
    s->field_0xd8 = 0;
    for (u32 i = 0; i < s->field_0x78; ++i) {
        UnkRefObject* p = s->field_0x70[i];
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
    s->field_0x78 = 0;
    UnkXParasiteTail_6d10* tail = reinterpret_cast<UnkXParasiteTail_6d10*>(reinterpret_cast<char*>(self) + 0x14710);
    s->field_0x20 = 0;
    s->field_0x28 = 0;
    s->field_0x30 = 0;
    s->field_0x38 = 0;
    s->field_0x3c = -1.0f;
    tail->c = 0;
    tail->a = 0;
    tail->b = 0;
    ResetRef(&s->field_0xe0);
    s->field_0xe8 = 0;
    s->field_0x1c = false;
    if (UnkXParasiteHolder_6d10* holder = self->owner->holder) {
        if (UnkXParasiteShape_6d10* shape = holder->shape) {
            u64 key = fn_7100001570("interior", 8);
            base::color::CColor4F color = {};
            fn_71001c28f8(shape->slots, key, 3, &color, 1);
        }
    }
}
