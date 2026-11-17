// Unit 1055 (tools/tu_map.py; no class hint): tracks `actor` by a weak handle in the list at +0x10 (once),
// and makes it the current one (+0x8) when there is none and no other tracked actor is closer to the
// actor's target (fn_7100d7c990). Returns whether the actor is the current one. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHandleNode_l6bd1e0 {
    UnkWeakHandle* handle;
    UnkHandleNode_l6bd1e0* next;

    explicit UnkHandleNode_l6bd1e0(UnkWeakHandle* h) : handle(nullptr) {
        AssignRef(&handle, h);
        next = nullptr;
    }
};

// A counted reference held while it is in use.
struct UnkHandleRef_l6bd1e0 {
    UnkWeakHandle* p;

    explicit UnkHandleRef_l6bd1e0(UnkWeakHandle* handle) : p(handle) {
        if (p)
            ++p->ref_count;
    }
    ~UnkHandleRef_l6bd1e0() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

static inline UnkWeakTarget* TargetOf(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

// A singly linked list of weak handles whose nodes come from its allocator.
struct UnkHandleList_l6bd1e0 {
    void* vtable;
    base::UnkLinkedList<UnkHandleNode_l6bd1e0> list;  // 0x8

    s32 IndexOf(const UnkHandleRef_l6bd1e0& handle) const {
        u32 i = 0;
        for (UnkHandleNode_l6bd1e0* node = list.head; node; node = node->next, ++i)
            if (TargetOf(node->handle) == TargetOf(handle.p))
                return static_cast<s32>(i);
        return -1;
    }

    void PushBack(const UnkHandleRef_l6bd1e0& handle) {
        void* mem = list.allocator->vf_0x40(sizeof(UnkHandleNode_l6bd1e0), 8, list.flags);
        if (mem)
            list.PushBack(new (mem) UnkHandleNode_l6bd1e0(handle.p));
    }
};

struct UnkActorTracker_l6bd1e0 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* current;          // 0x8
    UnkHandleList_l6bd1e0 tracked;  // 0x10
};

struct UnkPos_l6bd1e0 {
    f32 x, y, z;
};

static inline UnkPos_l6bd1e0 PosOf(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline f32 DistanceSq(const UnkPos_l6bd1e0& a, const UnkPos_l6bd1e0& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    f32 dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

// Whether another tracked actor is closer to `to` than `actor` is.
static inline bool AnyCloser(UnkActorTracker_l6bd1e0* self, UnkActor* actor, UnkActor* to) {
    UnkHandleNode_l6bd1e0* node = self->tracked.list.head;
    if (!node)
        return false;
    UnkPos_l6bd1e0 center = PosOf(to);
    f32 own = DistanceSq(PosOf(actor), center);
    for (; node; node = node->next) {
        UnkWeakHandle* handle = node->handle;
        if (!handle)
            continue;
        UnkActor* other = reinterpret_cast<UnkActor*>(handle->target);
        if (!other || other == actor)
            continue;
        if (DistanceSq(PosOf(other), center) < own)
            return true;
    }
    return false;
}

bool fn_71006bd1e0(UnkActorTracker_l6bd1e0* self, UnkActor* actor) {
    if (!actor)
        return false;
    UnkWeakTarget* target = reinterpret_cast<UnkWeakTarget*>(actor);
    if (self->tracked.IndexOf(UnkHandleRef_l6bd1e0(fn_7100d78378(target))) < 0)
        self->tracked.PushBack(UnkHandleRef_l6bd1e0(fn_7100d78378(target)));
    if (!TargetOf(self->current) && fn_7100d7c990(actor)) {
        if (!AnyCloser(self, actor, fn_7100d7c990(actor)))
            AssignRef(&self->current, fn_7100d78378(target));
    }
    UnkWeakHandle* current = self->current;
    return current && current->target && current->target == target;
}
