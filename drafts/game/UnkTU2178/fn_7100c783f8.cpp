// Unit 2178: removes the object a weak handle refers to from a set of weak references. While the set is
// being walked the removal is deferred: it cancels a pending addition, or else is queued itself.
// Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHandleVec_c783f8 {
    UnkWeakHandle** data;  // 0x0
    u32 count;             // 0x8
    u32 capacity;          // 0xc
    u32 step;              // 0x10
    u8 pad_0x14[0x28 - 0x14];

    UnkWeakHandle** begin() { return data; }
    UnkWeakHandle** end() { return data + count; }

    // Moves the last element into slot `index` and drops the last one.
    void EraseAt(u32 index);
    void PushBack(UnkWeakHandle* const& handle);
};

void fn_7100864cec(UnkHandleVec_c783f8* vec, u32 capacity);

struct UnkWeakSet_c783f8 {
    u8 pad_0x0[0x10];
    UnkHandleVec_c783f8 items;    // 0x10
    bool walking;                 // 0x38
    u8 pad_0x39[7];
    UnkHandleVec_c783f8 added;    // 0x40
    UnkHandleVec_c783f8 removed;  // 0x68
};
static_assert(__builtin_offsetof(UnkWeakSet_c783f8, removed) == 0x68, "layout");

static inline void Release(UnkWeakHandle* p) {
    if (p) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
}

inline void UnkHandleVec_c783f8::EraseAt(u32 index) {
    if (index >= count)
        return;
    u32 last = count - 1;
    if (index < last) {
        Release(data[index]);
        data[index] = nullptr;
        AssignRef(&data[index], data[last]);
    }
    for (u64 i = last; i < count; ++i)
        Release(data[i]);
    count = last;
}

inline void UnkHandleVec_c783f8::PushBack(UnkWeakHandle* const& handle) {
    u32 n = count;
    u32 c = capacity;
    if (c <= n && step != 0)
        c = (n / step + 1) * step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    fn_7100864cec(this, c);
    UnkWeakHandle** slot = &data[count];
    *slot = nullptr;
    AssignRef(slot, handle);
    ++count;
}

static inline UnkWeakTarget* TargetOf(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

static inline UnkWeakHandle** Find(UnkHandleVec_c783f8* vec, UnkWeakHandle** ref) {
    u32 n = vec->count;
    UnkWeakHandle** it = vec->data;
    for (; n != 0; --n, ++it) {
        if (TargetOf(*it) == TargetOf(*ref))
            break;
    }
    return it;
}

void fn_7100c783f8(void* self_, UnkWeakHandle** ref) {
    UnkWeakSet_c783f8* self = static_cast<UnkWeakSet_c783f8*>(self_);
    if (self->walking) {
        UnkWeakHandle** it = Find(&self->added, ref);
        if (it != self->added.end()) {
            self->added.EraseAt(it - self->added.data);
            return;
        }
        self->removed.PushBack(*ref);
    } else {
        UnkWeakHandle** it = Find(&self->items, ref);
        if (it != self->items.end())
            self->items.EraseAt(it - self->items.data);
    }
}
