// Unit 2178 (tools/tu_map.py): adds a weak handle to a handler list. While the list is being walked
// (the flag at +0x38), an add cancels a pending removal of the same object or is queued instead.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using UnkHandleVector_lc77fec = base::global::CRntVector<UnkWeakHandle*>;

// Grows the array to hold at least `capacity` handles.
void fn_7100864cec(UnkHandleVector_lc77fec* self, u32 capacity);

struct UnkHandlerList_lc77fec {
    u8 unk_0x0[0x10];
    UnkHandleVector_lc77fec handlers;  // 0x10
    bool iterating;                    // 0x38
    UnkHandleVector_lc77fec pendingAdds;     // 0x40
    UnkHandleVector_lc77fec pendingRemoves;  // 0x68
};

static inline UnkWeakTarget* TargetOf(UnkWeakHandle* handle) { return handle ? handle->target : nullptr; }

static inline void Release(UnkWeakHandle* handle) {
    if (handle) {
        if (--handle->ref_count == 0)
            handle->vf_0x10();
    }
}

// Copies a handle into a slot, taking a reference to it.
static inline void Assign(UnkWeakHandle** slot, UnkWeakHandle* handle) {
    if (handle)
        ++handle->ref_count;
    Release(*slot);
    *slot = handle;
}

static inline void Append(UnkHandleVector_lc77fec* v, UnkWeakHandle** ref) {
    u32 n = v->count;
    u32 new_capacity = v->capacity;
    if (new_capacity <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_7100864cec(v, new_capacity);
    UnkWeakHandle** slot = &v->data[v->count];
    *slot = nullptr;
    Assign(slot, *ref);
    ++v->count;
}

static inline UnkWeakHandle** Find(UnkHandleVector_lc77fec* v, UnkWeakHandle** ref) {
    UnkWeakHandle** it = v->data;
    for (u64 n = v->count; n != 0; --n, ++it) {
        if (TargetOf(*it) == TargetOf(*ref))
            return it;
    }
    return &v->data[v->count];
}

static inline void RemoveAt(UnkHandleVector_lc77fec* v, u32 index) {
    if (v->count > index) {
        u32 last = v->count - 1;
        if (last > index) {
            Release(v->data[index]);
            v->data[index] = nullptr;
            Assign(&v->data[index], v->data[last]);
        }
        for (u64 i = last; i < v->count; ++i)
            Release(v->data[i]);
        v->count = last;
    }
}

void fn_7100c77fec(void* self, UnkWeakHandle** ref) {
    UnkHandlerList_lc77fec* list = static_cast<UnkHandlerList_lc77fec*>(self);
    if (!list->iterating) {
        Append(&list->handlers, ref);
        return;
    }
    UnkWeakHandle** it = Find(&list->pendingRemoves, ref);
    if (it != &list->pendingRemoves.data[list->pendingRemoves.count])
        RemoveAt(&list->pendingRemoves, it - list->pendingRemoves.data);
    else
        Append(&list->pendingAdds, ref);
}
