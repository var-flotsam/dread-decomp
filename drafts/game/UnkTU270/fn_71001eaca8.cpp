// Unit 270: the destructor of the pool of 25000 placed entries: frees the buffer of seen entries, clears and frees
// the tree, then destroys every entry (box, then sphere), last first. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

void fn_7100215e1c(char* box);
void fn_7100216934(char* p);

struct UnkPoolEntry_l1eaca8 {
    u8 pad_0x0[0x48];
    char sphere[0x50];  // 0x48
    char box[0x20];     // 0x98
    u8 pad_0xb8[0x28];
};
static_assert(sizeof(UnkPoolEntry_l1eaca8) == 0xe0, "entry layout");

struct UnkTreeNode_l1eaca8 {
    char box[0x20];
    u8 pad_0x20[0x20];
    ~UnkTreeNode_l1eaca8() { fn_7100215e1c(box); }
};

void fn_710021273c(base::global::CRntVector<UnkTreeNode_l1eaca8>* self);

extern char g_unk_7101a1eeb0[];  // the pool's vtable

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
        fn_7100083990()->vf_0x48(self->seen);
        self->seen = nullptr;
    }
    fn_710021273c(&self->tree);
    self->tree.Free();
    for (u32 i = 25000; i != 0; --i) {
        UnkPoolEntry_l1eaca8& entry = self->entries[i - 1];
        fn_7100215e1c(entry.box);
        fn_7100216934(entry.sphere);
    }
}
