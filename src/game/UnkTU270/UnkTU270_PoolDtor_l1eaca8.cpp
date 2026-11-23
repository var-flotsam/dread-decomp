// Unit 270: the destructor of the pool of 25000 placed entries: frees the buffer of seen entries, clears and frees
// the tree, then destroys every entry (box, then sphere), last first. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU270.h"
#include "game/UnkTU307.h"
#include "types.h"

struct UnkPoolBox;  // the box class (src/game/UnkTU270/UnkTU270_Pool_ovb0254.cpp)

// An entry's sphere and box, each with an out-of-line destructor.
struct UnkPoolSphere_l1eaca8 {
    char data[0x50];
    ~UnkPoolSphere_l1eaca8() { fn_7100216934(data); }
};

struct UnkPoolBox_l1eaca8 {
    char data[0x20];
    ~UnkPoolBox_l1eaca8() { fn_7100215e1c(reinterpret_cast<UnkPoolBox*>(data)); }
};

struct UnkPoolEntry_l1eaca8 {
    u8 pad_0x0[0x48];
    UnkPoolSphere_l1eaca8 sphere;  // 0x48
    UnkPoolBox_l1eaca8 box;        // 0x98
    u8 pad_0xb8[0x28];
};
static_assert(sizeof(UnkPoolEntry_l1eaca8) == 0xe0, "entry layout");

struct UnkTreeNode_l1eaca8 {
    char box[0x20];
    u8 pad_0x20[0x20];
    ~UnkTreeNode_l1eaca8() { fn_7100215e1c(reinterpret_cast<UnkPoolBox*>(box)); }
};

void fn_710021273c(base::global::CRntVector<UnkTreeNode_l1eaca8>* self);

struct UnkPool_l1eaca8 {
    char* vtable;
    void* slots[25000];
    UnkPoolEntry_l1eaca8 entries[25000];
    void* tableA[25000];
    void* tableB[25000];
    void** seen;  // 0x5e9ac8
    u8 pad_0x5e9ad0[0xb8];
    base::global::CRntVector<UnkTreeNode_l1eaca8> tree;  // 0x5e9b88

};
static_assert(__builtin_offsetof(UnkPool_l1eaca8, seen) == 0x5e9ac8, "pool layout");
static_assert(__builtin_offsetof(UnkPool_l1eaca8, tree) == 0x5e9b88, "pool layout");

void fn_71001eaca8(UnkPool_l1eaca8* self) {
    self->vtable = g_unk_7101a1eeb0 + 0x10;
    if (self->seen) {
        GetDefaultAllocator()->vf_0x48(self->seen);
        self->seen = nullptr;
    }
    fn_710021273c(&self->tree);
    self->tree.Free();
    // The entries, last first.
    UnkPoolEntry_l1eaca8* entry = &self->entries[25000];
    do {
        --entry;
        entry->~UnkPoolEntry_l1eaca8();
    } while (entry != self->entries);
}
