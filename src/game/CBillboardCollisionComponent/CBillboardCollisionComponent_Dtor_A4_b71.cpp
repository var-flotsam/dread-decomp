// CBillboardCollisionComponent's unit (tools/tu_map.py unit 1151): a destructor of a class with a second
// base at +0x40 (vtable 0x7101a6d048, second table at +0x1b8): a callback link at +0x1b8, plain arrays at
// +0x190 and +0x168, a keyed table at +0x140, refcounted pointers at +0x138 and +0x130, then the base's
// destructor fn_7100860364. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/UnkTU883.h"
#include "types.h"

extern void* g_unk_7101a6d048[];
void fn_7100860364(void*);  // the base's destructor

namespace {

// A callback link: the list it is registered with, and two words cleared with it.
template <u32 Offset>
struct UnkBbLink_A4_b71 {
    static void Release(char* self) {
        void** link = reinterpret_cast<void**>(self + Offset);
        if (*link) {
            fn_71005c4828(*link, link);
            *link = nullptr;
        }
        link[1] = nullptr;
        link[2] = nullptr;
    }
};

template <u32 Offset>
struct UnkBbVec_A4_b71 {
    static void Release(char* self) { reinterpret_cast<base::UnkVector<u32>*>(self + Offset)->Free(); }
};

struct UnkBbTable_A4_b71 {
    void* data;
    void* buckets;
    u32 count;
    u32 field_0x14;
    u64 field_0x18;
    base::UnkAllocator* allocator;
};

template <u32 Offset>
struct UnkBbTableRel_A4_b71 {
    static void Release(char* self) {
        auto* t = reinterpret_cast<UnkBbTable_A4_b71*>(self + Offset);
        t->count = 0;
        if (t->buckets)
            t->allocator->vf_0x48(t->buckets);
        if (t->data)
            t->allocator->vf_0x48(t->data);
    }
};

}  // namespace

__attribute__((flatten)) void fn_7100723b6c(char* self) {
    char* vt = reinterpret_cast<char*>(g_unk_7101a6d048);
    *reinterpret_cast<char**>(self) = vt + 0x10;
    *reinterpret_cast<char**>(self + 0x40) = vt + 0x1b8;
    base::UnkMembers<UnkBbLink_A4_b71<0x1b8>, UnkBbVec_A4_b71<0x190>, UnkBbVec_A4_b71<0x168>,
                     UnkBbTableRel_A4_b71<0x140>, base::UnkRefPtrAt<0x138>,
                     base::UnkRefPtrAt<0x130>>::Release(self);
    fn_7100860364(self);
}
