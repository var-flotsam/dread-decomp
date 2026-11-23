// Unit 2506: grows an array of handle groups (a count and up to 128 refcounted handles each) and its
// parallel handle array to `capacity`, copying the handles over and releasing the old ones.
// Names and layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "game/UnkGroupArray_ovb0244.h"
#include "types.h"

// A counted reference to a handle; a new one is cleared by assigning an empty reference.
struct UnkHandleRef_ld4ae50 {
    UnkRefCountedU_ovb0244* object;
};

#pragma pack(push, 4)
struct UnkHandleGroup_ld4ae50 {
    u32 count;
    UnkHandleRef_ld4ae50 items[128];
};
#pragma pack(pop)

using UnkHandleGroupArray_ld4ae50 = base::global::CRntSmallDictionary<UnkRefCountedU_ovb0244*, UnkHandleGroup_ld4ae50>;

static inline void Release(UnkRefCountedU_ovb0244* h) {
    if (h) {
        if (--h->ref_count == 0)
            h->Delete();
    }
}

// Constructs `*dst` as a new reference to `src`'s handle.
static inline void CopyHandle(UnkHandleRef_ld4ae50* dst, const UnkHandleRef_ld4ae50& src) {
    *dst = UnkHandleRef_ld4ae50();
    if (UnkRefCountedU_ovb0244* h = src.object) {
        ++h->ref_count;
        Release(dst->object);
        dst->object = h;
    }
}

// Copies the handles of another group.
static inline void CopyGroup(UnkHandleGroup_ld4ae50* dst, UnkHandleGroup_ld4ae50* src) {
    for (u64 j = 0; j < src->count; ++j)
        CopyHandle(&dst->items[j], src->items[j]);
    dst->count = src->count;
}

// Releases every handle of a group and empties it.
static inline void ClearGroup(UnkHandleGroup_ld4ae50* group) {
    for (u64 j = 0; j < group->count; ++j)
        Release(group->items[j].object);
    group->count = 0;
}

void fn_7100d4ae50(UnkHandleGroupArray_ld4ae50* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkHandleGroup_ld4ae50* groups = static_cast<UnkHandleGroup_ld4ae50*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkHandleGroup_ld4ae50), 8, self->flags));
    UnkRefCountedU_ovb0244** handles =
        static_cast<UnkRefCountedU_ovb0244**>(self->allocator->vf_0x40((u64)capacity * 8, 8, self->flags));
    UnkHandleGroup_ld4ae50* old_groups = self->values;
    if (old_groups) {
        if (self->count) {
            UnkHandleGroup_ld4ae50* dst = groups;
            UnkHandleGroup_ld4ae50* src = old_groups;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                CopyGroup(dst, src);
                ClearGroup(src);
            }
            old_groups = self->values;
        }
        self->allocator->vf_0x48(old_groups);
    }
    UnkRefCountedU_ovb0244** old_handles = self->keys;
    if (old_handles) {
        if (self->count) {
            u64 idx = 0;
            for (u32 i = 0; i < self->count; ++i, ++idx) {
                handles[idx] = nullptr;
                if (UnkRefCountedU_ovb0244* h = old_handles[idx]) {
                    ++h->ref_count;
                    if (UnkRefCountedU_ovb0244* prev = handles[idx]) {
                        if (--prev->ref_count == 0)
                            prev->Delete();
                    }
                    handles[idx] = h;
                    if (UnkRefCountedU_ovb0244* o = old_handles[idx]) {
                        if (--o->ref_count == 0)
                            o->Delete();
                    }
                }
            }
            old_handles = self->keys;
        }
        self->allocator->vf_0x48(old_handles);
    }
    self->values = groups;
    self->keys = handles;
    self->capacity = capacity;
}
