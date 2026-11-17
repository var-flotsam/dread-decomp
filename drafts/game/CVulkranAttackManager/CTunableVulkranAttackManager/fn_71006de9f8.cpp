// CVulkranAttackManager::CTunableVulkranAttackManager's unit (tools/tu_map.py): adds `target` to the attackers list
// (once), makes it the current attacker when there is none, the cooldown is over and no other listed attacker is
// nearer to its own target, and tells whether `target` is the current attacker. Names and layouts are placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHandleNode_l6de9f8 {
    UnkWeakHandle* handle;
    UnkHandleNode_l6de9f8* next;

    explicit UnkHandleNode_l6de9f8(UnkWeakHandle* h) : handle(nullptr) {
        AssignRef(&handle, h);
        next = nullptr;
    }
};

// A counted reference held while it is in use.
struct UnkHandleRef_l6de9f8 {
    UnkWeakHandle* p;

    explicit UnkHandleRef_l6de9f8(UnkWeakHandle* handle) : p(handle) {
        if (p)
            ++p->ref_count;
    }
    ~UnkHandleRef_l6de9f8() {
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
struct UnkHandleList_l6de9f8 {
    void* vtable;
    base::UnkLinkedList<UnkHandleNode_l6de9f8> list;  // 0x8

    s32 IndexOf(const UnkHandleRef_l6de9f8& handle) const {
        s32 index = 0;
        for (UnkHandleNode_l6de9f8* node = list.head; node; node = node->next, ++index)
            if (TargetOf(node->handle) == TargetOf(handle.p))
                return index;
        return -1;
    }

    void PushBack(const UnkHandleRef_l6de9f8& handle) {
        void* mem = list.allocator->vf_0x40(sizeof(UnkHandleNode_l6de9f8), 8, list.flags);
        if (mem)
            list.PushBack(new (mem) UnkHandleNode_l6de9f8(handle.p));
    }
};
static_assert(sizeof(UnkHandleList_l6de9f8) == 0x30, "layout");

struct UnkVulkranAttackers_l6de9f8 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* current;          // 0x8
    f32 cooldown;                    // 0x10
    u8 pad_0x14[4];
    UnkHandleList_l6de9f8 attackers;  // 0x18
};

static inline UnkWeakTarget* AsTarget(UnkActor* actor) {
    return reinterpret_cast<UnkWeakTarget*>(actor);
}

struct UnkPos_l6de9f8 {
    f32 x, y, z;
};

static inline UnkPos_l6de9f8 PositionOf(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline f32 DistanceSq(const UnkActor* actor, const UnkPos_l6de9f8& from) {
    f32 dx = actor->field_0x44 - from.x;
    f32 dy = actor->field_0x54 - from.y;
    f32 dz = actor->field_0x64 - from.z;
    return dx * dx + dy * dy + dz * dz;
}

bool fn_71006de9f8(UnkVulkranAttackers_l6de9f8* self, UnkActor* attacker) {
    if (!attacker)
        return false;

    if (self->attackers.IndexOf(UnkHandleRef_l6de9f8(fn_7100d78378(AsTarget(attacker)))) < 0)
        self->attackers.PushBack(UnkHandleRef_l6de9f8(fn_7100d78378(AsTarget(attacker))));

    if (!TargetOf(self->current) && self->cooldown <= 0.001f && fn_7100d7c990(attacker)) {
        UnkActor* victim = fn_7100d7c990(attacker);
        bool nearer = false;
        if (self->attackers.list.head) {
            UnkPos_l6de9f8 from = PositionOf(victim);
            f32 distance = DistanceSq(attacker, from);
            for (UnkHandleNode_l6de9f8* node = self->attackers.list.head; node; node = node->next) {
                if (!node->handle)
                    continue;
                UnkActor* other = reinterpret_cast<UnkActor*>(node->handle->target);
                if (!other || other == attacker)
                    continue;
                if (DistanceSq(other, from) < distance) {
                    nearer = true;
                    break;
                }
            }
        }
        if (!nearer)
            AssignRef(&self->current, fn_7100d78378(AsTarget(attacker)));
    }

    UnkWeakHandle* current = self->current;
    if (!current)
        return false;
    UnkWeakTarget* target = current->target;
    bool valid = target != nullptr;
    bool same = target == AsTarget(attacker);
    return same & valid;
}
