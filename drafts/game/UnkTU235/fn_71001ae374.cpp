// Unit 235 (tools/tu_map.py): fn_71001ae374 moves a node's pointers from one block to another. The node's
// own two pointers go from `from` to `to`; the groups it holds (by its kind) are reached at the context's
// base, their leaves' pointers moved from `from` to `to`, and the groups and leaves then moved from the
// base to `to`. Names and layouts are placeholders.
#include "types.h"

struct UnkRelocContext_l1ae374 {
    const char* base;
};

struct UnkRelocLeaf_l1ae374 {
    u8 pad_0x0[0x18];
    void* field_0x18;
};

// The groups of leaves a node holds, one type for each member.
struct UnkRelocGroup0_l1ae374 {
    UnkRelocLeaf_l1ae374* leaves[2];
};

struct UnkRelocGroup1_l1ae374 {
    UnkRelocLeaf_l1ae374* leaves[1];
};

struct UnkRelocGroup2_l1ae374 {
    UnkRelocLeaf_l1ae374* leaves[2];
};

struct UnkRelocGroup3_l1ae374 {
    UnkRelocLeaf_l1ae374* leaves[4];
};

struct UnkRelocNode_l1ae374 {
    void* field_0x0;
    void* field_0x8;
    u32 kind;
    u8 pad_0x14[0x28 - 0x14];
    union {
        struct {
            UnkRelocGroup0_l1ae374* first;   // 0x28
            UnkRelocGroup1_l1ae374* second;  // 0x30
            UnkRelocGroup2_l1ae374* third;   // 0x38
        } kind0;
        struct {
            UnkRelocGroup3_l1ae374* group;  // 0x28
        } kind1;
    };
};

// A pointer into the block at `from` made to point at the same place in the block at `to`.
template <class T>
static inline void Rebase(T*& p, const char* from, const char* to) {
    if (p && from != to)
        p = reinterpret_cast<T*>(reinterpret_cast<char*>(p) + (to - from));
}

template <class Group>
static void RebaseGroup(Group*& group, const char* base, const char* from, const char* to) {
    if (!group)
        return;
    Rebase(group, from, base);
    for (UnkRelocLeaf_l1ae374*& leaf : group->leaves) {
        if (leaf) {
            Rebase(leaf, from, base);
            Rebase(leaf->field_0x18, from, to);
            Rebase(leaf, base, to);
        }
    }
    Rebase(group, base, to);
}

void fn_71001ae374(UnkRelocContext_l1ae374* context, UnkRelocNode_l1ae374* node, const char* from,
                   const char* to) {
    const char* base = context->base;
    Rebase(node->field_0x0, from, to);
    Rebase(node->field_0x8, from, to);
    switch (node->kind) {
    case 0:
        RebaseGroup(node->kind0.first, base, from, to);
        RebaseGroup(node->kind0.second, base, from, to);
        RebaseGroup(node->kind0.third, base, from, to);
        break;
    case 1:
        RebaseGroup(node->kind1.group, base, from, to);
        break;
    }
}
