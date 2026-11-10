// CTeamManager::CTunableTeamManager's unit (tools/tu_map.py unit 2738): the team manager's two lists of
// weak references to actors. The first (at +0x0) keeps one node per member with a value (a distance,
// FLT_MAX when unknown) and a flag; the second (at +0x70) keeps the same members sorted by a comparator
// object (or by value), rebuilt from the first. Both lists allocate their nodes from a block pool of
// their own. Names and layouts are placeholders.
#include <cfloat>
#include <math.h>
#include <new>

#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);
struct UnkPlayer;
struct UnkDropCameraVec3;
const UnkDropCameraVec3* fn_7100d7c9f4(UnkPlayer* player);
struct UnkComponent_71011058a8;
bool fn_71011058a8(UnkComponent_71011058a8* self);

struct UnkRefl_fn_710031caf8;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_710031caf8>(void*);

extern void* g_unk_7101cb55e8[];  // the member list's vtable
extern void* g_unk_7101cb5608[];  // the base part's of the member list
extern void* g_unk_7101cb5628[];  // the sorted list's vtable
extern void* g_unk_7101cb5648[];  // the pooled part's of the sorted list
extern void* g_unk_7101cb5668[];  // the base part's of the sorted list

// The AI component of a member actor.
struct UnkTeamAI {
    virtual void vf_0x0();
    u8 pad_0x8[0x1140];
    void* field_0x1148;  // what it is doing for the team

    void vf_0x220(bool on) { reinterpret_cast<void (***)(UnkTeamAI*, bool)>(this)[0][0x220 / 8](this, on); }
    bool vf_0x228() { return reinterpret_cast<bool (***)(UnkTeamAI*)>(this)[0][0x228 / 8](this); }
};

bool fn_7101105d80(UnkTeamAI* ai);

struct UnkTeamBusy {
    u8 pad_0x0[0x1a0];
    void* field_0x1a0;
};

struct UnkTeamActor {
    u8 pad_0x0[0x240];
    UnkTeamAI* ai;  // +0x240
    u8 pad_0x248[0x30];
    UnkTeamBusy* busy;  // +0x278
};

static inline UnkTeamActor* AsActor(UnkWeakTarget* target) {
    return reinterpret_cast<UnkTeamActor*>(target);
}

// A counted reference to a weak handle.
struct UnkTeamRef {
    UnkWeakHandle* ptr;

    explicit UnkTeamRef(UnkWeakHandle* p) : ptr(nullptr) { AssignRef(&ptr, p); }
    UnkTeamRef(const UnkTeamRef& o) : ptr(nullptr) { AssignRef(&ptr, o.ptr); }
    ~UnkTeamRef() {
        if (UnkWeakHandle* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
    UnkWeakTarget* Target() const { return ptr ? ptr->target : nullptr; }
};

static inline UnkWeakHandle* HandleOf(UnkWeakTarget* target) {
    return target ? fn_7100d78378(target) : nullptr;
}

// A member and its value, as the comparator sees it.
struct UnkTeamKey {
    UnkTeamRef handle;
    f32 value;

    UnkTeamKey(const UnkTeamKey& o) : handle(o.handle), value(o.value) {}
};

// A comparator object: vf_0x10 tells whether `a` goes before `b`. The ones at fn_71011a7cb4 and
// fn_71011a7d88 forward to a function pointer.
struct UnkTeamCompare {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual bool vf_0x10(UnkTeamKey a, UnkTeamKey b);
    bool (*fn)(UnkTeamKey a, UnkTeamKey b);
};

// A block pool: a free list over one buffer taken from `parent`.
struct UnkTeamPool {
    char* vtable;
    base::UnkAllocator* parent;
    UnkBlockFreeList blocks;

    __attribute__((always_inline)) void Destroy() {
        vtable = g_unk_7101a1a4c8 + 0x10;
        if (void* buffer = blocks.buffer) {
            fn_71000837f4(&blocks);
            parent->vf_0x48(buffer);
            parent = nullptr;
        }
    }
};

template <class List>
__attribute__((always_inline)) inline void FreeNodes(List* list) {
    for (auto* n = list->head; n;) {
        auto* next = n->next;
        n->Ref().~UnkTeamRef();
        list->allocator->vf_0x48(n);
        n = next;
    }
}

struct UnkTeamNode {
    UnkTeamRef handle;
    f32 value;
    bool flag;
    UnkTeamNode* next;

    explicit UnkTeamNode(const UnkTeamRef& h) : handle(h), value(FLT_MAX), flag(false), next(nullptr) {}
    UnkWeakTarget* Target() const { return handle.Target(); }
    UnkTeamRef& Ref() { return handle; }
};

// The member list (0x70 bytes).
struct UnkTeamList {
    void** vtable;
    u32 count;
    UnkTeamNode* head;
    UnkTeamNode* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    UnkTeamPool pool;  // +0x30
    u32 field_0x68;
    bool field_0x6c;

    void PushBack(UnkTeamNode* node) {
        if (count == 0) {
            head = node;
            tail = node;
        } else {
            // Stored untyped: the original reloads the count after it.
            __builtin_memcpy(&tail->next, &node, sizeof(node));
            node->next = nullptr;
            tail = node;
        }
        ++count;
    }

    // Flags the member `actor` when its AI component is doing something for the team.
    bool MarkBusy(UnkWeakTarget* actor) {
        for (UnkTeamNode* n = head; n; n = n->next) {
            if (n->Target() == actor) {
                UnkTeamActor* a = AsActor(n->handle.ptr->target);
                if (a->ai && a->ai->field_0x1148) {
                    n->flag = true;
                    return true;
                }
            }
        }
        return false;
    }
};
static_assert(sizeof(UnkTeamList) == 0x70, "UnkTeamList layout");

struct UnkTeamSortedNode {
    UnkTeamKey key;
    UnkTeamSortedNode* next;
    UnkTeamSortedNode* prev;

    explicit UnkTeamSortedNode(const UnkTeamKey& k) : key(k), next(nullptr), prev(nullptr) {}
    UnkTeamRef& Ref() { return key.handle; }
};

// The sorted list (0x80 bytes).
struct UnkTeamSortedList {
    void** vtable;
    u32 count;
    UnkTeamSortedNode* head;
    UnkTeamSortedNode* tail;
    u32 flags;
    base::UnkAllocator* allocator;
    base::UnkAllocator* pool_source;
    UnkTeamPool pool;  // +0x38
    u32 field_0x70;
    bool field_0x74;
    UnkTeamCompare* compare;
};
static_assert(sizeof(UnkTeamSortedList) == 0x80, "UnkTeamSortedList layout");

struct UnkTeamManager {
    UnkTeamList list;
    UnkTeamSortedList sorted;  // +0x70
    u8 pad_0xf0[0x10];
    UnkWeakHandle* field_0x100;
};

// Rebuilds the sorted list from the members whose AI component is active (fn_71011a6ec8 for each).
void fn_71011a6d50(UnkTeamManager* self);

void fn_71011a67b8(UnkTeamSortedList* self, u32 capacity, u32 flags, base::UnkAllocator* allocator) {
    self->vtable = &g_unk_7101cb5668[2];
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    self->flags = flags;
    self->allocator = allocator ? allocator : fn_7100083990();
    self->pool_source = nullptr;
    self->vtable = &g_unk_7101cb5648[2];
    self->pool.vtable = g_unk_7101a1a4c8 + 0x10;
    self->pool.parent = nullptr;
    fn_710008377c(&self->pool.blocks);
    self->field_0x70 = 0;
    self->field_0x74 = false;
    base::UnkAllocator* source = self->pool_source ? self->pool_source : self->allocator;
    u32 pool_flags = self->flags;
    if (void* buffer = self->pool.blocks.buffer) {
        fn_71000837f4(&self->pool.blocks);
        self->pool.parent->vf_0x48(buffer);
        self->pool.parent = nullptr;
    }
    if (capacity) {
        if (!source)
            source = fn_7100083990();
        self->pool.parent = source;
        u64 size = (u64)capacity * sizeof(UnkTeamSortedNode);
        void* buffer = source->vf_0x40(size, 8, pool_flags);
        fn_710008378c(&self->pool.blocks, buffer, size, sizeof(UnkTeamSortedNode));
    }
    // The nodes come from the pool from now on.
    self->allocator = reinterpret_cast<base::UnkAllocator*>(reinterpret_cast<char*>(self) +
                                                            __builtin_offsetof(UnkTeamSortedList, pool));
    self->vtable = &g_unk_7101cb5628[2];
    self->compare = nullptr;
}

void fn_71011a69b8(UnkTeamList* self) {
    self->vtable = &g_unk_7101cb55e8[2];
    self->field_0x6c = true;
    FreeNodes(self);
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    self->allocator = nullptr;
    self->pool.Destroy();
    self->vtable = &g_unk_7101cb5608[2];
    FreeNodes(self);
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}

// The member whose value is the smallest.
UnkWeakTarget* fn_71011a6b68(UnkTeamList* self) {
    UnkWeakTarget* best = nullptr;
    f32 min = FLT_MAX;
    for (UnkTeamNode* n = self->head; n; n = n->next) {
        if (min > n->value) {
            best = n->Target();
            min = n->value;
        }
    }
    return best;
}

// The planar distance from each member to the actor it is acting against.
void fn_71011a6c9c(UnkTeamList* self) {
    for (UnkTeamNode* n = self->head; n; n = n->next) {
        if (!n->handle.ptr || !n->handle.ptr->target)
            continue;
        UnkActor* other = fn_7100d7c990(reinterpret_cast<UnkActor*>(n->handle.ptr->target));
        if (!other)
            continue;
        CVector2D a, b;
        fn_710007b430(&a, reinterpret_cast<const CVector3D*>(fn_7100d7c9f4(reinterpret_cast<UnkPlayer*>(other))));
        fn_710007b430(&b, reinterpret_cast<const CVector3D*>(
                              fn_7100d7c9f4(reinterpret_cast<UnkPlayer*>(n->Target()))));
        f32 dx = a.x - b.x;
        f32 dy = a.y - b.y;
        n->value = sqrtf(dx * dx + dy * dy);
    }
}

// Inserts `key` before the first node it goes before (by the comparator, or by value without one).
void fn_71011a6ec8(UnkTeamSortedList* self, const UnkTeamKey* key) {
    void* mem = self->allocator->vf_0x40(sizeof(UnkTeamSortedNode), 8, self->flags);
    UnkTeamSortedNode* node = mem ? new (mem) UnkTeamSortedNode(*key) : nullptr;
    if (self->count == 0) {
        self->head = node;
        self->tail = node;
        node->next = nullptr;
        node->prev = nullptr;
    } else {
        for (UnkTeamSortedNode* cur = self->head; cur; cur = cur->next) {
            if ((self->compare && self->compare->vf_0x10(cur->key, node->key)) ||
                (!self->compare && cur->key.value < node->key.value)) {
                if (cur == self->tail) {
                    cur->next = node;
                    node->next = nullptr;
                    node->prev = cur;
                    self->tail = node;
                    break;
                }
                continue;
            }
            if (cur == self->head) {
                self->head = node;
                node->prev = nullptr;
            }
            if (cur->prev) {
                // Stored untyped: the original reloads cur->prev after it.
                __builtin_memcpy(&cur->prev->next, &node, sizeof(node));
                node->prev = cur->prev;
            }
            node->next = cur;
            cur->prev = node;
            break;
        }
    }
    ++self->count;
}

// Resets the members' values and flags, then flags those that the sorted list's leading entries
// (the ones with a positive value) have started something for.
void fn_71011a7110(UnkTeamManager* self) {
    for (UnkTeamNode* n = self->list.head; n; n = n->next) {
        if (UnkWeakTarget* t = n->Target()) {
            UnkTeamActor* actor = AsActor(t);
            if (actor->busy && actor->busy->field_0x1a0)
                continue;
            UnkTeamAI* ai = actor->ai;
            if (ai && ai->vf_0x228())
                continue;
            if (fn_7101105d80(ai))
                ai->vf_0x220(true);
        } else {
            n->value = FLT_MAX;
        }
        n->flag = false;
    }
    fn_71011a6c9c(&self->list);
    fn_71011a6d50(self);
    for (UnkTeamSortedNode* n = self->sorted.head; n && n->key.value > 0.0f; n = n->next) {
        UnkWeakTarget* t = n->key.handle.Target();
        if (!t)
            continue;
        UnkTeamAI* ai = AsActor(t)->ai;
        if (!ai || !fn_71011058a8(reinterpret_cast<UnkComponent_71011058a8*>(ai)))
            continue;
        self->list.MarkBusy(t);
    }
}

void fn_71011a726c(UnkTeamList* self, UnkWeakTarget* target) {
    bool found = false;
    for (UnkTeamNode* n = self->head; n; n = n->next) {
        bool eq = n->Target() == target;
        found |= eq;
        if (eq)
            break;
    }
    if (found)
        return;
    UnkTeamRef ref(HandleOf(target));
    void* mem = self->allocator->vf_0x40(sizeof(UnkTeamNode), 8, self->flags);
    if (mem)
        self->PushBack(new (mem) UnkTeamNode(ref));
}

static inline void SetActive(UnkTeamList* self, UnkWeakTarget* target, bool on) {
    UnkTeamAI* ai = reinterpret_cast<UnkTeamAI*>(base::reflection::refl_FindTyped<UnkRefl_fn_710031caf8>(target));
    if (!ai)
        return;
    if (target) {
        for (UnkTeamNode* n = self->head; n; n = n->next) {
            if (n->Target() == target) {
                n->flag = false;
                break;
            }
        }
    }
    if (fn_7101105d80(ai))
        ai->vf_0x220(on);
}

void fn_71011a7554(UnkTeamList* self, UnkWeakTarget* target) {
    SetActive(self, target, false);
}

void fn_71011a75f0(UnkTeamList* self, UnkWeakTarget* target) {
    SetActive(self, target, true);
}

void fn_71011a7730(UnkTeamManager* self, UnkWeakTarget* target) {
    AssignRef(&self->field_0x100, HandleOf(target));
}

void fn_71011a7a18(UnkTeamSortedList* self) {
    self->vtable = &g_unk_7101cb5648[2];
    self->field_0x74 = true;
    FreeNodes(self);
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    self->pool.vtable = g_unk_7101a1a4c8 + 0x10;
    self->allocator = nullptr;
    self->pool.Destroy();
    self->vtable = &g_unk_7101cb5668[2];
    FreeNodes(self);
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}

bool fn_71011a7cb4(UnkTeamCompare* self, UnkTeamKey a, UnkTeamKey b) {
    if (!self->fn)
        return false;
    return self->fn(a, b);
}

bool fn_71011a7d88(UnkTeamCompare* self, UnkTeamKey a, UnkTeamKey b) {
    if (!self->fn)
        return false;
    return self->fn(a, b);
}
