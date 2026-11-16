// CScenario unit: empties the scenario's actor lists at +0x2a0 and +0x310, telling each actor that is not
// disabled, not placed in one of the three layers and not an enabled, visible CActorDef (vtable +0xe0).
// The first list's actors are also unregistered first. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkScenarioObject_ovb0032;
struct UnkScenario_ovb0032;
// As src/game/CScenario/CScenario_Actors_ovb0032.cpp declares it.

struct UnkLayerEntry_l1a1e34 {
    u8 pad_0x0[0x60];
};

struct UnkActorDef_l1a1e34 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x58];
    u8 field_0x60;
    u8 pad_0x61[0x67];
    u8 field_0xc8;
};
static_assert(__builtin_offsetof(UnkActorDef_l1a1e34, field_0xc8) == 0xc8, "UnkActorDef_l1a1e34 layout");

struct UnkScenarioActor_l1a1e34 {
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
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();

    u8 pad_0x8[0x20];
    base::reflection::UnkReflObject* def;  // 0x28
    u8 pad_0x30[0x98];
    UnkLayerEntry_l1a1e34* layer_entry;  // 0xc8
    u8 pad_0xd0[0xe5];
    u8 field_0x1b5;
};
static_assert(__builtin_offsetof(UnkScenarioActor_l1a1e34, field_0x1b5) == 0x1b5, "UnkScenarioActor_l1a1e34 layout");

struct UnkScenarioLayer_l1a1e34 {
    UnkLayerEntry_l1a1e34* entries;
    u8 pad_0x8[0x8];
    u32 count;  // 0x10
    u8 pad_0x14[0xb4];
};
static_assert(sizeof(UnkScenarioLayer_l1a1e34) == 0xc8, "UnkScenarioLayer_l1a1e34 layout");

struct UnkScenarioListTag_l1a1e34;
using UnkScenarioList_l1a1e34 = base::UnkSList<UnkScenarioListTag_l1a1e34>;

struct UnkScenario_l1a1e34 {
    u8 pad_0x0[0x28];
    UnkScenarioLayer_l1a1e34 layers[3];  // 0x28
    u8 pad_0x280[0x20];
    UnkScenarioList_l1a1e34 list_0x2a0;
    u8 pad_0x2d0[0x40];
    UnkScenarioList_l1a1e34 list_0x310;
    u8 pad_0x340[0xb0];
    bool field_0x3f0;
};
static_assert(__builtin_offsetof(UnkScenario_l1a1e34, list_0x2a0) == 0x2a0, "UnkScenario_l1a1e34 layout");
static_assert(__builtin_offsetof(UnkScenario_l1a1e34, field_0x3f0) == 0x3f0, "UnkScenario_l1a1e34 layout");

static inline UnkActorDef_l1a1e34* AsActorDef(base::reflection::UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), fn_7100254b14()) ? static_cast<UnkActorDef_l1a1e34*>(obj) : nullptr;
}

static inline bool IsInLayer(const UnkScenarioLayer_l1a1e34& layer, const UnkScenarioActor_l1a1e34* actor) {
    for (const UnkLayerEntry_l1a1e34 *it = layer.entries, *end = layer.entries + layer.count; it != end; ++it)
        if (it == actor->layer_entry)
            return true;
    return false;
}

static inline bool IsPlaced(UnkScenario_l1a1e34* self, UnkScenarioActor_l1a1e34* actor) {
    for (const UnkScenarioLayer_l1a1e34& layer : self->layers)
        if (IsInLayer(layer, actor))
            return true;
    return false;
}

// Whether the actor's definition is an enabled CActorDef that is not hidden.
static inline bool IsShownDef(UnkScenarioActor_l1a1e34* actor) {
    return actor->def && fn_710009d50c(actor->def->vf_0x0(), fn_7100254b14()) &&
           AsActorDef(actor->def)->field_0xc8 && !AsActorDef(actor->def)->field_0x60;
}

// Frees the nodes of a list, leaving it empty.
static inline void FreeNodes(UnkScenarioList_l1a1e34& list) {
    for (UnkScenarioList_l1a1e34::Node* n = list.head; n;) {
        UnkScenarioList_l1a1e34::Node* next = n->next;
        list.allocator->vf_0x48(n);
        n = next;
    }
    list.count = 0;
    list.head = nullptr;
    list.tail = nullptr;
}

void fn_71011a1e34(UnkScenario_l1a1e34* self) {
    self->field_0x3f0 = true;
    for (UnkScenarioList_l1a1e34::Node* n = self->list_0x2a0.head; n; n = n->next) {
        UnkScenarioActor_l1a1e34* actor = reinterpret_cast<UnkScenarioActor_l1a1e34*>(n->value);
        fn_71002337d0(reinterpret_cast<UnkScenarioObject_ovb0032*>(actor), nullptr);
        if (!actor->field_0x1b5 && !IsPlaced(self, actor) && !IsShownDef(actor))
            actor->vf_0xe0();
    }
    FreeNodes(self->list_0x2a0);

    self->field_0x3f0 = false;
    for (UnkScenarioList_l1a1e34::Node* n = self->list_0x310.head; n; n = n->next) {
        UnkScenarioActor_l1a1e34* actor = reinterpret_cast<UnkScenarioActor_l1a1e34*>(n->value);
        if (!actor->field_0x1b5 && !IsPlaced(self, actor) && !IsShownDef(actor))
            actor->vf_0xe0();
    }
    FreeNodes(self->list_0x310);
}
