// Unit 2702 (hint CAIComponent): the AI component's destructor: its members, last first, then the base
// component's destructor (fn_71010c4194). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkEmptyDtorElem.h"
#include "base/UnkNodeOwner.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityGhostAura.h"
#include "game/CInputComponent.h"
#include "game/UnkCallbackSetVector.h"
#include "game/UnkComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkTU362.h"
#include "game/UnkTU39.h"
#include "game/UnkTU883.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101cb1dd0[];  // the AI component's vtable

struct UnkListSets_l0f9264;
void fn_71010f9264(UnkListSets_l0f9264* self);
void fn_710088c874(void* set, void** slot);
void fn_71011146c0(UnkCallbackSetVector* v, u32 size);
void fn_7101046ac0(void* blackboard);
void CCharClassEventScenarioComponent_ResizeStrIdList(void* self, bool flag);
void fn_71010d78e4(void* self);

template <u32 Next>
__attribute__((always_inline)) inline void UnkDestroyEmptyNode(UnkListNode<Next>* node) {
    fn_71002874dc(node);
}
void fn_71010f9550(UnkNodeOwner<UnkListNode<0x20>, UnkDestroyEmptyNode<0x20>>* self);

struct UnkAICallbacks_l0f8b98;
template <>
struct base::UnkCallbackTraits<UnkAICallbacks_l0f8b98> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

// A pool-backed list (src/game/CAIComponent/CAIComponent_PoolList_b44.cpp).
extern char g_unk_7101cb25b8[];
extern char g_unk_7101cb25d8[];

struct UnkWidePoolListBase_b44a3 {
    char* vtable;
    u32 count;
    char* head;
    char* tail;
    u64 field_0x20;
    base::UnkAllocator* allocator;
    u32 flags;

    ~UnkWidePoolListBase_b44a3() {
        vtable = g_unk_7101cb25d8 + 0x10;
        FreeNodes();
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
    void FreeNodes() {
        for (char* n = head; n;) {
            char* next = *reinterpret_cast<char**>(n + 0x18);
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

struct UnkWidePoolList_b44a3 : UnkWidePoolListBase_b44a3 {
    base::UnkBlockPool pool;  // 0x38

    ~UnkWidePoolList_b44a3() {
        vtable = g_unk_7101cb25b8 + 0x10;
        pool.field_0x3c = true;
        FreeNodes();
        count = 0;
        __builtin_memset(&head, 0, 0x10);
        allocator = nullptr;
    }
};
static_assert(sizeof(UnkWidePoolList_b44a3) == 0x78, "UnkWidePoolList_b44a3 placeholder layout");

// A slot in a callback set, with the object and callback it runs; Detach takes it out of the set.
template <void (*Detach)(void*, void**)>
struct UnkLink_l0f8b98 {
    void* set;
    void* object;
    void* callback;

    ~UnkLink_l0f8b98() {
        if (set) {
            Detach(set, &set);
            set = nullptr;
        }
        callback = nullptr;
        object = nullptr;
    }
};

static inline void DetachDeferred(void* set, void** slot) {
    fn_71002ae87c(set, slot);
}

// A fixed-capacity array with its count in front (4-byte aligned).
template <class T, u32 N>
struct __attribute__((packed, aligned(4))) UnkFixedArray_l0f8b98 {
    u32 count;
    T items[N];
};

using UnkIdGroup_l0f8b98 = UnkFixedArray_l0f8b98<CStrId, 8>;

static void ReleaseIds(CStrId* ids, u32* count) {
    for (u64 i = 0; i < *count; ++i)
        ids[i].~CStrId();
    *count = 0;
}

static void ReleaseGroups(UnkFixedArray_l0f8b98<UnkIdGroup_l0f8b98, 3>* groups) {
    for (u64 i = 0; i < groups->count; ++i) {
        UnkIdGroup_l0f8b98* group = &groups->items[i];
        for (u64 j = 0; j < group->count; ++j)
            group->items[j].~CStrId();
        group->count = 0;
    }
    groups->count = 0;
}

// A point on the navigation mesh.
struct UnkNavPoint_l0f8b98 {
    u8 data[0x14];

    ~UnkNavPoint_l0f8b98() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

struct UnkTargetPoint_l0f8b98 {
    u8 data[0xc];

    ~UnkTargetPoint_l0f8b98() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

struct UnkWatchTarget_l0f8b98 {
    u8 pad_0x0[0x30];
    struct {
        u8 pad_0x0[0x1d8];
        void* list;
    }* owner;
};

struct UnkWatchEntry_l0f8b98 {
    ~UnkWatchEntry_l0f8b98() {}
    u32 value;
};

// The listener's base part: a name and an array.
struct UnkWatchBase_l0f8b98 {
    void** vtable;
    CRntString name;  // 0x8
    u8 pad_name[0x40 - 0x8 - sizeof(CRntString)];
    UnkWatchEntry_l0f8b98* entries;  // 0x40

    ~UnkWatchBase_l0f8b98() {
        vtable = &g_unk_7101a22878[2];
        if (entries) {
            delete[] entries;
            entries = nullptr;
        }
    }
};

// A listener registered with its target's +0x30 object's list.
struct UnkWatch_l0f8b98 : UnkWatchBase_l0f8b98 {
    UnkWatchTarget_l0f8b98* target;  // 0x48

    ~UnkWatch_l0f8b98() {
        vtable = &g_unk_7101a22858[2];
        if (target && target->owner && target->owner->list)
            fn_7100872908(target->owner->list, this);
        target = nullptr;
    }
};
static_assert(sizeof(UnkWatch_l0f8b98) == 0x50, "layout");

// The counted link a listener holds; the object it is registered with is at +0x10.
struct UnkWeakLink_l0f8b98 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    void* owner;
};

// A listener that unregisters from its link's owner (fn_710117b3f0) on destruction.
struct UnkWeakListener_l0f8b98 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakLink_l0f8b98* link;

    ~UnkWeakListener_l0f8b98() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkWeakLink_l0f8b98* l = link) {
            bool held = true;
            if (void* owner = l->owner) {
                fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), this);
                l = link;
                held = l != nullptr;
            }
            if (held && --l->ref_count == 0)
                l->Release();
        }
    }
};

struct UnkMapNode_l0f8b98 {
    u8 pad_0x0[0x8];
    UnkMapNode_l0f8b98* next;
};

// A hash map: its nodes, linked from `head`, and the bucket array.
struct UnkMap_l0f8b98 {
    base::UnkAllocator* allocator;
    u8 pad_0x8[0x4];
    u32 count;  // 0xc
    CRntVector<void*> buckets;  // 0x10
    UnkMapNode_l0f8b98* tail;  // 0x38
    UnkMapNode_l0f8b98* head;  // 0x40

    ~UnkMap_l0f8b98() {
        if (count) {
            for (UnkMapNode_l0f8b98* n = head; n;) {
                UnkMapNode_l0f8b98* next = n->next;
                allocator->vf_0x48(n);
                n = next;
            }
            __builtin_memset(buckets.data, 0, buckets.capacity * sizeof(void*));
            count = 0;
            tail = nullptr;
            head = nullptr;
        }
        buckets.Free();
    }
};

struct UnkAIComponentD_l0f8b98 {
    void** vtable;
    u8 pad_0x8[0x60];
    u8 field_0x68[0x1100];
    UnkWidePoolList_b44a3 pool_list;  // 0x1168
    u8 pad_0x11e0[0x44];
    u8 field_0x1224[0x3c];
    u8 blackboard[0xf0];  // 0x1260
    base::UnkCallbackListShort<UnkAICallbacks_l0f8b98> callbacks;  // 0x1350
    UnkCallbackSetVector sets;  // 0x1390
    u8 pad_0x13b8[0x28];
    UnkFixedArray_l0f8b98<UnkIdGroup_l0f8b98, 3> id_groups;  // 0x13e0
    UnkFixedArray_l0f8b98<CStrId, 13> ids;  // 0x14b0
    u8 pad_0x151c[0x4];
    UnkNodeOwner<UnkListNode<0x20>, UnkDestroyEmptyNode<0x20>> nodes;  // 0x1520
    u8 pad_nodes[0x15b0 - 0x1520 - sizeof(UnkNodeOwner<UnkListNode<0x20>, UnkDestroyEmptyNode<0x20>>)];
    base::UnkRefPtrMember shared;  // 0x15b0
    u8 pad_0x15b8[0x30];
    UnkLink_l0f8b98<fn_71005c4828> area_link;  // 0x15e8
    UnkTargetPoint_l0f8b98 target_point;  // 0x1600
    u8 pad_0x160c[0x4];
    void* tunable;  // 0x1610
    u32 field_0x1618;
    u8 pad_0x1620[0xc8];
    CRntVector<void*> list_0x16e8;
    u8 pad_0x1710[0x20];
    CRntVector<void*> list_0x1730;
    u8 pad_0x1758[0x28];
    CStrId name;  // 0x1780
    u8 pad_0x1788[0x20];
    UnkWatch_l0f8b98 watch;  // 0x17a8
    UnkLink_l0f8b98<DetachDeferred> deferred_a;  // 0x17f8
    UnkLink_l0f8b98<DetachDeferred> deferred_b;  // 0x1810
    u8 list_sets[0x180];  // 0x1828
    UnkLink_l0f8b98<fn_710088c874> hook_link;  // 0x19a8
    u8 pad_0x19c0[0x8];
    u8 path[0x12a88];  // 0x19c8
    UnkNavPoint_l0f8b98 points[3];  // 0x14450
    u8 pad_0x1448c[0xc];
    UnkWeakListener_l0f8b98 listener;  // 0x14498
    UnkMap_l0f8b98 map;  // 0x144b8
    u8 pad_0x14500[0x20];
    CRntVector<void*> list_0x14520;
    CRntVector<void*> list_0x14548;
    u8 pad_0x14570[0x8];
    u8 field_0x14578[0x8];
};
static_assert(__builtin_offsetof(UnkAIComponentD_l0f8b98, callbacks) == 0x1350, "layout");
static_assert(__builtin_offsetof(UnkAIComponentD_l0f8b98, id_groups) == 0x13e0, "layout");
static_assert(__builtin_offsetof(UnkAIComponentD_l0f8b98, ids) == 0x14b0, "layout");
static_assert(__builtin_offsetof(UnkAIComponentD_l0f8b98, shared) == 0x15b0, "layout");
static_assert(__builtin_offsetof(UnkAIComponentD_l0f8b98, list_0x16e8) == 0x16e8, "layout");
static_assert(__builtin_offsetof(UnkAIComponentD_l0f8b98, name) == 0x1780, "layout");
static_assert(__builtin_offsetof(UnkAIComponentD_l0f8b98, hook_link) == 0x19a8, "layout");
static_assert(__builtin_offsetof(UnkAIComponentD_l0f8b98, points) == 0x14450, "layout");
static_assert(__builtin_offsetof(UnkAIComponentD_l0f8b98, map) == 0x144b8, "layout");
static_assert(__builtin_offsetof(UnkAIComponentD_l0f8b98, field_0x14578) == 0x14578, "layout");

static inline void FreeSets(UnkCallbackSetVector* sets) {
    if (sets->data) {
        fn_71011146c0(sets, 0);
        sets->allocator->vf_0x48(sets->data);
        sets->capacity = 0;
    }
}

void fn_71010f8b98(UnkAIComponentD_l0f8b98* self) {
    self->field_0x1618 = 0;
    self->vtable = &g_unk_7101cb1dd0[2];
    fn_710002f67c(self->field_0x14578);
    self->list_0x14548.Free();
    self->list_0x14520.Free();
    self->map.~UnkMap_l0f8b98();
    self->listener.~UnkWeakListener_l0f8b98();
    self->points[2].~UnkNavPoint_l0f8b98();
    self->points[1].~UnkNavPoint_l0f8b98();
    self->points[0].~UnkNavPoint_l0f8b98();
    fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(self->path));
    self->hook_link.~UnkLink_l0f8b98();
    fn_71010f9264(reinterpret_cast<UnkListSets_l0f9264*>(self->list_sets));
    self->deferred_b.~UnkLink_l0f8b98();
    self->deferred_a.~UnkLink_l0f8b98();
    self->watch.~UnkWatch_l0f8b98();
    self->name.~CStrId();
    self->list_0x1730.Free();
    self->list_0x16e8.Free();
    self->field_0x1618 = 0;
    self->target_point.~UnkTargetPoint_l0f8b98();
    self->area_link.~UnkLink_l0f8b98();
    self->shared.~UnkRefPtrMember();
    fn_71010f9550(&self->nodes);
    ReleaseIds(self->ids.items, &self->ids.count);
    ReleaseGroups(&self->id_groups);
    FreeSets(&self->sets);
    self->callbacks.~UnkCallbackListShort();
    fn_7101046ac0(self->blackboard);
    CCharClassEventScenarioComponent_ResizeStrIdList(self->field_0x1224, false);
    self->pool_list.~UnkWidePoolList_b44a3();
    fn_71010d78e4(self->field_0x68);
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
