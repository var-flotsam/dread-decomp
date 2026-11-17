// Unit 270 (tools/tu_map.py), the pool of placed entries: the entries seen from a view, found by walking
// the tree of boxes for the first table (up to `max`) and taking all of the second table, then dropping
// those whose sphere is outside the view. Names and layouts are placeholders.
#include <string.h>

#include "game/UnkTU58.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkPlaneSet_b0258g;
struct UnkBounds_b0258g;
bool fn_71001ba188(UnkPlaneSet_b0258g* self, UnkBounds_b0258g* b);

// A node of the tree (0x40 bytes): its box, and a link that is negative for a leaf (the low 27 bits
// index the first table) or else the next node to visit when the box is not in the view.
struct UnkTreeNode_l1ebe80 {
    u8 box[0x38];
    s32 link;  // 0x38
    u32 pad_0x3c;
};
static_assert(sizeof(UnkTreeNode_l1ebe80) == 0x40, "node layout");

// Whether the node's box is in the view.
bool fn_71001b823c(UnkPlaneSet_b0258g* view, UnkTreeNode_l1ebe80* node, void* arg);

template <class T>
struct UnkRntVector_l1ebe80 {
    T* data;
    u32 size;

    T& operator[](u32 i) {
        if (size <= i)
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return data[i];
    }
};

// An object an entry stands for: flagged when it wants to be told it was seen, and the frame it was
// last told.
struct UnkSeenObject_l1ebe80 {
    u8 unk_0x0[0x354];
    u32 lastFrame;  // 0x354
    u8 unk_0x358[9];
    u8 wantsSeen;  // 0x361
};

struct UnkPoolEntry_l1ebe80 {
    UnkSeenObject_l1ebe80* object;
    u8 pad_0x8[0x40];
    u8 sphere[0x50];  // 0x48
};

constexpr u32 kPoolSize_l1ebe80 = 25000;

struct UnkPool_l1ebe80 {
    u8 pad_0x0[0x588048];
    UnkPoolEntry_l1ebe80* tableA[kPoolSize_l1ebe80];  // 0x588048
    UnkPoolEntry_l1ebe80* tableB[kPoolSize_l1ebe80];  // 0x5b8d88
    u8 pad_0x5e9ac8[0xb8];
    u32 countB;  // 0x5e9b80
    u8 pad_0x5e9b84[0x4];
    UnkRntVector_l1ebe80<UnkTreeNode_l1ebe80> tree;  // 0x5e9b88
};
static_assert(__builtin_offsetof(UnkPool_l1ebe80, tableB) == 0x5b8d88, "pool layout");
static_assert(__builtin_offsetof(UnkPool_l1ebe80, tree) == 0x5e9b88, "pool layout");

// The sixth parameter is not read.
void fn_71001ebe80(UnkPool_l1ebe80* self, UnkPlaneSet_b0258g* view, UnkPoolEntry_l1ebe80** out1, u32 max,
                   void** out2, u64, u32* count1, u32* count2) {
    *count1 = 0;
    *count2 = 0;
    u32 frame = g_unk_7101d58e48;

    u32 n = self->tree.size;
    u32 k = 0;
    u32 i = 0;
    while (i < n) {
        UnkTreeNode_l1ebe80* node = &self->tree[i];
        bool visible = node->link < 0 || fn_71001b823c(view, node, nullptr);
        if (visible && node->link < 0) {
            if (k + 1 < max)
                out1[k++] = self->tableA[node->link & 0x7ffffff];
        }
        // A node not in the view skips its subtree.
        if (!visible && node->link >= 0)
            i = node->link;
        else
            ++i;
    }
    *count1 = k;

    memcpy(&out1[k], self->tableB, self->countB * sizeof(UnkPoolEntry_l1ebe80*));
    *count1 += self->countB;

    for (u32 i = 0; i < *count1;) {
        UnkPoolEntry_l1ebe80* entry = out1[i];
        if (fn_71001ba188(view, reinterpret_cast<UnkBounds_b0258g*>(entry->sphere))) {
            UnkSeenObject_l1ebe80* object = entry->object;
            if (object->wantsSeen) {
                u32 old = object->lastFrame;
                object->lastFrame = frame;
                if (old != frame) {
                    u32 j = *count2;
                    *count2 = j + 1;
                    out2[j] = object;
                }
            }
            ++i;
        } else {
            if (*count1 > 1)
                out1[i] = out1[*count1 - 1];
            --*count1;
        }
    }
}
