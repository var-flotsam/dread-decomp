// animtree::CAnimTreeNode unit (tools/tu_map.py, the name from reflection): stores a value at +0x60 of
// the child at `index`, then refreshes it (fn_71002df91c) when asked. The index is a full 32-bit
// argument, but src/game/animtree/CLayerNode/CLayerNode_Update.cpp declares it `bool`: this definition
// keeps that symbol through an asm label. Names and layouts are placeholders.
#include "types.h"

struct UnkLayerNode;

struct UnkAnimTreeChild_b66a2 {
    u8 pad_0x0[0x60];
    f32 weight;
};

struct UnkAnimTreeNodeView_b66a2 {
    u8 pad_0x0[0x38];
    UnkAnimTreeChild_b66a2** children;
};

void fn_71002df91c(UnkAnimTreeChild_b66a2* child, bool flag);

void fn_71002dfaf0_b66a2(UnkLayerNode* node, u32 index, f32 dt, bool b)
    asm("_Z13fn_71002dfaf0P12UnkLayerNodebfb");

void fn_71002dfaf0_b66a2(UnkLayerNode* node, u32 index, f32 dt, bool b) {
    UnkAnimTreeChild_b66a2* child = reinterpret_cast<UnkAnimTreeNodeView_b66a2*>(node)->children[index];
    child->weight = dt;
    if (b) {
        fn_71002df91c(child, true);
    }
}
