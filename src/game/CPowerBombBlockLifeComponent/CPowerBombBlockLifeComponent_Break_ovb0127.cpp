// CPowerBombBlockLifeComponent's unit (tools/tu_map.py): its destructor (the two listeners, the
// string-id table at +0x250, the id at +0x248, then the base destructor), and the model and
// collider updates that show or hide the parts whose names contain "_BREAK". Names and layouts are
// placeholders.
#include "base/UnkStrIdTable.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/CAbilityGhostAura.h"
#include "game/CLifeComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CPowerBombBlockLifeComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A listener holding a counted link to what it listens to; on destruction it detaches itself from
// the link's target (with `detach`) and drops the link.
struct UnkPowerBombListener_ovb0127 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakHandle* link;  // 0x18

    template <class F>
    __attribute__((always_inline)) void Destroy(void** vt, F detach) {
        vtable = vt;
        if (UnkWeakHandle* l = link) {
            bool held = true;
            if (UnkWeakTarget* target = l->target) {
                detach(target, this);
                l = link;
                held = l != nullptr;
            }
            if (held && --l->ref_count == 0)
                l->vf_0x10();
        }
    }
};

struct UnkPowerBombBlockLife_ovb0127 {
    void** vtable;
    u8 pad_0x8[0x240];
    base::global::CStrId field_0x248;
    UnkStrIdTable field_0x250;
    UnkPowerBombListener_ovb0127 field_0x278;
    UnkPowerBombListener_ovb0127 field_0x298;
};

static void DetachA(UnkWeakTarget* target, void* listener) { fn_710117b3f0(target, listener); }
static void DetachB(UnkWeakTarget* target, void* listener) { fn_7100a2d4d0(target, listener); }

void fn_71008e3034(UnkPowerBombBlockLife_ovb0127* self) {
    self->vtable = g_unk_7101a7f280 + 2;
    self->field_0x298.Destroy(g_unk_7101a22d48 + 2, DetachA);
    self->field_0x278.Destroy(g_unk_7101a77000 + 2, DetachB);
    UnkStrIdTable* table = &self->field_0x250;
    fn_71004f11f8(table);
    if (table->keys)
        table->allocator->vf_0x48(table->keys);
    if (table->values)
        table->allocator->vf_0x48(table->values);
    self->field_0x248.Reset();
    fn_710115f91c(self);
}

// A string id's entry (base::global::UnkStringEntry in base/global/UnkStringPoolBody.h) as these
// functions see it: the use count at +0x44 is a union member, as its store makes the original reload
// the entry pointer.
struct UnkPowerBombNameEntry_ovb0127 {
    u8 pad_0x0[0x10];
    base::global::CRntString buffer;  // 0x10
    u8 pad_0x40[0x4];
    union {
        u32 uses;  // 0x44
    };
};
using UnkStringEntry = UnkPowerBombNameEntry_ovb0127;

struct UnkStruct_7100233684;
base::reflection::UnkReflObject* fn_7100233684(UnkStruct_7100233684*);
struct UnkFlagTarget;

// A named part: its name's string entry first (a string id).
struct UnkPowerBombPart_ovb0127 {
    UnkStringEntry* name;
    const base::global::CStrId& id() const { return *reinterpret_cast<const base::global::CStrId*>(this); }
};

struct UnkPowerBombPartList_ovb0127;
u32 fn_71001c1440(UnkPowerBombPartList_ovb0127* list);
UnkPowerBombPart_ovb0127* fn_71001c1450(UnkPowerBombPartList_ovb0127* list, u32 index);

struct UnkPowerBombModelData_ovb0127 {
    u8 pad_0x0[0x138];
    UnkPowerBombPartList_ovb0127* parts;  // 0x138
};

struct UnkPowerBombModel_ovb0127 {
    u8 pad_0x0[0x50];
    UnkPowerBombModelData_ovb0127* data;  // 0x50
};

// Whether the name contains `str` (an empty name doesn't); reading it counts a use of the entry.
static inline bool NameHas(UnkStringEntry* const& name, const char* str) {
    ++name->uses;
    if (name == reinterpret_cast<UnkStringEntry*>(&g_unk_7101d47108->empty))
        return false;
    return fn_710007fb9c(&name->buffer, str);
}

static inline void ShowPart(UnkPowerBombModel_ovb0127* model, UnkPowerBombPart_ovb0127* part) {
    s64 key = reinterpret_cast<UnkStringEntry*>(part->id().data)->buffer.key;
    fn_7101172764(reinterpret_cast<UnkFlagTarget*>(model), reinterpret_cast<const char*>(&key));
}

static inline void HidePart(UnkPowerBombModel_ovb0127* model, UnkPowerBombPart_ovb0127* part) {
    s64 key = reinterpret_cast<UnkStringEntry*>(part->id().data)->buffer.key;
    fn_7101172780(reinterpret_cast<UnkFlagTarget*>(model), reinterpret_cast<const char*>(&key));
}

struct UnkPowerBombBlockLifeParts_ovb0127 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkStruct_7100233684* owner;  // 0x30
};

void fn_71008e36d0(UnkPowerBombBlockLifeParts_ovb0127* self) {
    auto* model = reinterpret_cast<UnkPowerBombModel_ovb0127*>(fn_7100233684(self->owner));
    if (!model)
        return;
    UnkPowerBombModelData_ovb0127* data = model->data;
    if (!data)
        return;
    for (u32 i = 0; i < fn_71001c1440(data->parts); ++i) {
        UnkPowerBombPart_ovb0127* part = fn_71001c1450(data->parts, i);
        if (NameHas(part->name, "_BREAK"))
            ShowPart(model, part);
        else if (!NameHas(part->name, "Root"))
            HidePart(model, part);
    }
}

struct UnkPowerBombCollider_ovb0127 {
    u8 pad_0x0[0x20];
    UnkStringEntry* name;  // 0x20
};

struct UnkPowerBombColliders_ovb0127 {
    u8 pad_0x0[0x50];
    u32 count;  // 0x50
};
UnkPowerBombCollider_ovb0127* fn_710086420c(UnkPowerBombColliders_ovb0127* self, u32 index);

struct UnkPowerBombOwner_ovb0127 {
    u8 pad_0x0[0x2a0];
    UnkPowerBombColliders_ovb0127* colliders;  // 0x2a0
};

struct UnkPowerBombBlockLifeBreak_ovb0127 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkPowerBombOwner_ovb0127* owner;  // 0x30
    void vf_0x140(CBlackboard* blackboard) {
        reinterpret_cast<void (*)(UnkPowerBombBlockLifeBreak_ovb0127*, CBlackboard*)>(vtable[0x140 / 8])(this, blackboard);
    }
};

void fn_71008e3808(UnkPowerBombBlockLifeBreak_ovb0127* self) {
    auto* model = reinterpret_cast<UnkPowerBombModel_ovb0127*>(
        fn_7100233684(reinterpret_cast<UnkStruct_7100233684*>(self->owner)));
    if (model) {
        if (UnkPowerBombModelData_ovb0127* data = model->data) {
            for (u32 i = 0; i < fn_71001c1440(data->parts); ++i) {
                UnkPowerBombPart_ovb0127* part = fn_71001c1450(data->parts, i);
                if (NameHas(part->name, "_BREAK"))
                    HidePart(model, part);
                else
                    ShowPart(model, part);
            }
        }
    }
    if (UnkPowerBombColliders_ovb0127* colliders = self->owner->colliders) {
        for (u32 i = 0; i < colliders->count; ++i) {
            UnkPowerBombCollider_ovb0127* collider = fn_710086420c(colliders, i);
            UnkStringEntry* const& name = collider->name;
            ++name->uses;
            bool solid;
            if (name != reinterpret_cast<UnkStringEntry*>(&g_unk_7101d47108->empty) &&
                fn_710007fb9c(&name->buffer, "_BREAK"))
                solid = false;
            else
                solid = true;
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), solid);
        }
    }
    self->vf_0x140(&g_unk_7101d47310->field_0x968);
}
