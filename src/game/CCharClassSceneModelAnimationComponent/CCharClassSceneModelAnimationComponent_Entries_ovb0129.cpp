// CCharClassSceneModelAnimationComponent unit (tools/tu_map.py): the 0x50-byte entries that fn_71009f26a0
// destroys (CCharClassSceneModelAnimationComponent_b50.cpp spells their layout): their default constructor
// (fn_71009f2814: empty string ids, no object, no deleter) and the array reserve (fn_71009f24c0) that moves
// each entry (string ids, owned object, std::function deleter) into the new storage and destroys the old one.
// Names and layouts are placeholders.
#include <functional>
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkEntry_b50a5;
void fn_71009f26a0(UnkEntry_b50a5* self);

// A string id as the entry's constructors build it: null, then the pool's empty string (the pool
// created first when missing); a move leaves the empty string behind.
struct UnkEntryId_b0129 {
    base::global::UnkStringData* data;

    UnkEntryId_b0129() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    UnkEntryId_b0129(UnkEntryId_b0129&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
};

struct UnkEntry_b0129 {
    UnkEntryId_b0129 id0;
    UnkEntryId_b0129 id1;
    void* object;
    u8 unk_0x18[0x8];
    std::function<void(void*)> deleter;

    UnkEntry_b0129() : object(nullptr) {}
    UnkEntry_b0129(UnkEntry_b0129&& other)
        : id0(static_cast<UnkEntryId_b0129&&>(other.id0)), id1(static_cast<UnkEntryId_b0129&&>(other.id1)),
          object(Take(other.object)), deleter(static_cast<std::function<void(void*)>&&>(other.deleter)) {}

    static void* Take(void*& p) {
        void* moved = p;
        p = nullptr;
        return moved;
    }
};
static_assert(sizeof(UnkEntry_b0129) == 0x50, "entry size");

struct UnkEntryArray_b0129 {
    UnkEntry_b0129* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_71009f24c0(UnkEntryArray_b0129* self, u32 n) {
    if (self->capacity < n) {
        UnkEntry_b0129* p =
            static_cast<UnkEntry_b0129*>(self->allocator->vf_0x40((u64)n * sizeof(UnkEntry_b0129), 8, self->flags));
        if (UnkEntry_b0129* old = self->data) {
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j) {
                new (&p[j]) UnkEntry_b0129(static_cast<UnkEntry_b0129&&>(old[j]));
                fn_71009f26a0(reinterpret_cast<UnkEntry_b50a5*>(&old[j]));
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
}

__attribute__((flatten)) void fn_71009f2814(UnkEntry_b0129* self) {
    new (self) UnkEntry_b0129;
}
