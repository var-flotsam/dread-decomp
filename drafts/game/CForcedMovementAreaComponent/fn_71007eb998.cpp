// CForcedMovementAreaComponent's unit (1256): takes in a collider that entered the area. When the entry
// keyed by its collision component's owner id lists the collider's target and the area does not hold it
// yet, appends the collider's weak handle to the area's list. Names and layouts are placeholders.
#include <algorithm>
#include <new>

#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAreaCollider_A5_b65;

// The collider as a weak-handle target: its entity (+0x10) and the id of what it belongs to (+0x20).
struct UnkCollider_l7eb998 : UnkWeakTarget {
    u64 field_0x8;
    UnkReflObject* entity;
    u64 field_0x18;
    u64 owner;
};

struct UnkKeyed_l7eb998 {
    u8 pad_0x0[0x30];
    u64 id;
};

// The entity as a collision component: what it is attached to (+0x30).
struct UnkCollision_l7eb998 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkKeyed_l7eb998* owner;
};

// The weak handle of a collider's target is what the lists hold: the colliders' targets.
static inline u64 OwnerOf(UnkWeakHandle* handle) {
    return reinterpret_cast<UnkCollider_l7eb998*>(handle->target)->owner;
}

// A counted reference to a weak handle, held while it is looked at or appended.
struct UnkHandleRef_l7eb998 {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_l7eb998(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHandleRef_l7eb998() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

// A counted reference to a handle of the area's list (which holds no nulls), held while it is looked at.
struct UnkHeldHandle_l7eb998 {
    UnkWeakHandle* handle;

    explicit UnkHeldHandle_l7eb998(UnkWeakHandle* p) : handle(p) { ++handle->ref_count; }
    ~UnkHeldHandle_l7eb998() {
        if (--handle->ref_count == 0)
            handle->vf_0x10();
    }
};

// A slot of the area's list: a counted reference to a weak handle.
struct UnkHandleSlot_l7eb998 {
    UnkWeakHandle* handle;

    void Assign(UnkWeakHandle* p) {
        if (p) {
            ++p->ref_count;
            if (UnkWeakHandle* old = handle) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            handle = p;
        }
    }
};

using UnkHandleList_l7eb998 = base::global::CRntVector<UnkWeakHandle*>;

void fn_71007ed3a4(UnkHandleList_l7eb998* list, u32 capacity);

struct UnkForcedMovementArea_A5_b65 {
    u8 pad_0x0[0x48];
    UnkHandleList_l7eb998* entries;  // one list of collider handles per key
    u64* keys;
    u32 count;
    u8 pad_0x5c[0x70 - 0x5c];
    UnkHandleList_l7eb998 inside;  // 0x70

    UnkHandleList_l7eb998* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &entries[i];
        return entries + count;
    }
};

static inline void AppendHandle(UnkHandleList_l7eb998& list, const UnkHandleRef_l7eb998& value) {
    u32 n = list.count;
    u32 new_capacity = list.capacity;
    if (new_capacity <= n && list.step != 0)
        new_capacity = (n / list.step + 1) * list.step;
    // A capacity that doesn't grow past the count (no step) hangs, as in the original.
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_71007ed3a4(&list, new_capacity);
    UnkHandleSlot_l7eb998* slot = new (&list.data[list.count]) UnkHandleSlot_l7eb998();
    slot->Assign(value.handle);
    ++list.count;
}

void fn_71007eb998(UnkForcedMovementArea_A5_b65* self, UnkAreaCollider_A5_b65* area_collider) {
    UnkCollider_l7eb998* collider = reinterpret_cast<UnkCollider_l7eb998*>(area_collider);
    UnkReflObject* entity = collider->entity;
    UnkCollision_l7eb998* collision =
        fn_710009d50c(entity->vf_0x0(), fn_710085fc44()) ? static_cast<UnkCollision_l7eb998*>(entity) : nullptr;
    UnkHandleList_l7eb998* entry = self->Find(collision->owner->id);
    if (entry == self->entries + self->count)
        return;

    UnkWeakHandle** end = entry->data + entry->count;
    UnkWeakHandle** listed = std::find_if(entry->data, end, [collider](UnkWeakHandle* handle) {
        return collider->owner == OwnerOf(handle);
    });

    for (UnkWeakHandle** it = self->inside.data, **inside_end = it + self->inside.count; it != inside_end; ++it) {
        UnkHeldHandle_l7eb998 held(*it);
        if (OwnerOf(held.handle) == collider->owner)
            return;
    }
    if (listed == entry->data + entry->count)
        return;

    UnkHandleRef_l7eb998 handle(GetWeakHandle(collider));
    AppendHandle(self->inside, handle);
}
