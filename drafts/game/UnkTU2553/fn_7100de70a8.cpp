// Unit 2553 (CGameManager::CTunableGameManager): drops the game manager's actors: releases the object at
// +0x7e8, then for each of the two actor maps detaches every actor that is not an enabled, visible
// CActorDef and erases it from the map by name, clears the second map and three lists, and resets the
// object at +0x2848. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkCloneRecords.h"
#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBombGun.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CStrId;

struct UnkActorTable_b43a3;
struct UnkActorWithDef_b43a3;
bool fn_710119dd94(UnkActorTable_b43a3* self, UnkActorWithDef_b43a3* actor);
void fn_7100d8bb70(void* self);

// An object released through its reference count (vtable +0x10 when it drops to zero).
struct UnkRefCounted_lde70a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 ref_count;  // 0x8
};

struct UnkActorDef_lde70a8 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x58];
    u8 field_0x60;
    u8 pad_0x61[0x67];
    u8 field_0xc8;
};
static_assert(__builtin_offsetof(UnkActorDef_lde70a8, field_0xc8) == 0xc8, "UnkActorDef_lde70a8 layout");

struct UnkActor_lde70a8 {
    u8 pad_0x0[0x28];
    base::reflection::UnkReflObject* def;  // 0x28
    CStrId name;                           // 0x30
    u8 pad_0x38[0x17a];
    bool field_0x1b2;
};
static_assert(__builtin_offsetof(UnkActor_lde70a8, field_0x1b2) == 0x1b2, "UnkActor_lde70a8 layout");

// The actors by name: a hash map whose nodes are also chained in a list from +0x40.
struct UnkActorMapNode_lde70a8 {
    CStrId name;
    UnkActor_lde70a8* actor;
    UnkActorMapNode_lde70a8* next;
};

struct UnkActorMap_lde70a8 {
    base::IAllocator* allocator;
    u32 pad_0x8;
    u32 count;                          // 0xc
    UnkActorMapNode_lde70a8** buckets;  // 0x10
    u32 pad_0x18;
    u32 bucket_count;                   // 0x1c
    u8 pad_0x20[0x18];
    UnkActorMapNode_lde70a8* field_0x38;
    UnkActorMapNode_lde70a8* head;  // 0x40
};
static_assert(sizeof(UnkActorMap_lde70a8) == 0x48, "UnkActorMap_lde70a8 layout");

struct UnkActorListNode_lde70a8 {
    UnkActor_lde70a8* actor;
    UnkActorListNode_lde70a8* next;
};
typedef base::UnkLinkedList<UnkActorListNode_lde70a8> UnkActorList_lde70a8;

struct UnkGameManagerActors_lde70a8 {
    u8 pad_0x0[0x7e8];
    UnkRefCounted_lde70a8* field_0x7e8;
    u8 pad_0x7f0[0x1e08];
    UnkActorMap_lde70a8 actors;        // 0x25f8
    UnkActorMap_lde70a8 field_0x2640;  // 0x2640
    u8 pad_0x2688[0x8];
    UnkActorList_lde70a8 list_0x2690;  // 0x2690
    u8 pad_0x26b8[0x48];
    UnkActorList_lde70a8 list_0x2700;  // 0x2700
    u8 pad_0x2728[0xb8];
    UnkActorList_lde70a8 list_0x27e0;  // 0x27e0
    u8 pad_0x2808[0x40];
    u8 field_0x2848;
};
static_assert(__builtin_offsetof(UnkGameManagerActors_lde70a8, actors) == 0x25f8, "layout");
static_assert(__builtin_offsetof(UnkGameManagerActors_lde70a8, list_0x2690) == 0x2690, "layout");
static_assert(__builtin_offsetof(UnkGameManagerActors_lde70a8, list_0x27e0) == 0x27e0, "layout");
static_assert(__builtin_offsetof(UnkGameManagerActors_lde70a8, field_0x2848) == 0x2848, "layout");

// The names of the actors being dropped.
struct UnkDroppedNames_lde70a8 : base::global::CRntVector<CStrId> {
    UnkDroppedNames_lde70a8() : CRntVector<CStrId>(0xf) {}
    ~UnkDroppedNames_lde70a8() asm("fn_71008600a8");

    // Grows the array to n names, moving the names over and freeing the old array.
    void Grow(u32 n) {
        if (capacity < n) {
            CStrId* p = static_cast<CStrId*>(allocator->vf_0x40((u64)n * sizeof(CStrId), 8, flags));
            if (data) {
                CStrId* src = data;
                CStrId* dst = p;
                for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                    CStrId::MoveConstructReloading(dst, *src);
                    src->~CStrId();
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }

    // Appends a copy of `name` (the string-id array's PushBack, which grows out of line).
    void Append(const CStrId& name) { reinterpret_cast<base::UnkStrIdVec*>(this)->PushBack(name); }
};

// The array as it starts: room for 100 names, growing by 100.
static UnkDroppedNames_lde70a8 NewDroppedNames() {
    UnkDroppedNames_lde70a8 names;
    names.step = 100;
    names.Grow(100);
    return names;
}

static inline UnkActorDef_lde70a8* AsActorDef(base::reflection::UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), fn_7100254b14()) ? static_cast<UnkActorDef_lde70a8*>(obj) : nullptr;
}

// Whether the actor's definition is an enabled CActorDef that is not hidden.
static inline bool IsShownDef(UnkActor_lde70a8* actor) {
    return actor->def && fn_710009d50c(actor->def->vf_0x0(), fn_7100254b14()) &&
           AsActorDef(actor->def)->field_0xc8 && !AsActorDef(actor->def)->field_0x60;
}

static inline UnkSubAreaGuiCache* ActorTable_lde70a8() {
    UnkSubAreaGuiCache* table = g_unk_71727254f8;
    if (!table) {
        table = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
        fn_710119d484(table);
        g_unk_71727254f8 = table;
    }
    return table;
}

// Frees every node of `map` and empties it.
static inline void ClearMap(UnkActorMap_lde70a8& map) {
    if (!map.count)
        return;
    for (UnkActorMapNode_lde70a8* node = map.head; node;) {
        UnkActorMapNode_lde70a8* next = node->next;
        node->name.~CStrId();
        map.allocator->vf_0x48(node);
        node = next;
    }
    memset(map.buckets, 0, (u64)map.bucket_count * sizeof(UnkActorMapNode_lde70a8*));
    map.count = 0;
    map.field_0x38 = nullptr;
    map.head = nullptr;
}

void fn_7100de70a8(UnkGameManager* manager) {
    auto* self = reinterpret_cast<UnkGameManagerActors_lde70a8*>(manager);
    if (UnkRefCounted_lde70a8* object = self->field_0x7e8) {
        if (--object->ref_count == 0)
            object->vf_0x10();
        self->field_0x7e8 = nullptr;
    }
    static UnkDroppedNames_lde70a8 s_names = NewDroppedNames();
    // Each map's actors that are not shown definitions are detached and erased from it by name.
    for (UnkActorMapNode_lde70a8* node = self->actors.head; node; node = node->next) {
        UnkActor_lde70a8* actor = node->actor;
        if (IsShownDef(actor))
            continue;
        s_names.Append(actor->name);
        if (actor->field_0x1b2)
            fn_71002329e8(reinterpret_cast<UnkActor_71002329e8*>(actor));
        if (!fn_710119dd94(reinterpret_cast<UnkActorTable_b43a3*>(ActorTable_lde70a8()),
                           reinterpret_cast<UnkActorWithDef_b43a3*>(actor)))
            fn_7100232124(reinterpret_cast<UnkBoundObj*>(actor));
    }
    for (CStrId *it = s_names.data, *end = s_names.data + s_names.count; it != end; ++it)
        fn_71002430e8(reinterpret_cast<UnkKeyMap_b60*>(&self->actors), reinterpret_cast<UnkKeyNode_b60*>(it));
    s_names.Clear();

    for (UnkActorMapNode_lde70a8* node = self->field_0x2640.head; node; node = node->next) {
        UnkActor_lde70a8* actor = node->actor;
        if (IsShownDef(actor))
            continue;
        s_names.Append(actor->name);
        if (actor->field_0x1b2)
            fn_71002329e8(reinterpret_cast<UnkActor_71002329e8*>(actor));
        if (!fn_710119dd94(reinterpret_cast<UnkActorTable_b43a3*>(ActorTable_lde70a8()),
                           reinterpret_cast<UnkActorWithDef_b43a3*>(actor)))
            fn_7100232124(reinterpret_cast<UnkBoundObj*>(actor));
    }
    for (CStrId *it = s_names.data, *end = s_names.data + s_names.count; it != end; ++it)
        fn_71002430e8(reinterpret_cast<UnkKeyMap_b60*>(&self->field_0x2640), reinterpret_cast<UnkKeyNode_b60*>(it));
    s_names.Clear();

    ClearMap(self->field_0x2640);
    self->list_0x2700.Clear();
    self->list_0x2690.Clear();
    self->list_0x27e0.Clear();
    fn_7100d8bb70(&self->field_0x2848);
}
