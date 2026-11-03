// Unit 212 of tools/tu_map.py (0x7100194a9c-0x710019545c, no reflected class): a search of a tree of
// named nodes (the layout CCutscene.cpp uses): the first child with a payload, depth first, or null.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkNamedNode {
    u8 pad_0x0[0x60];
    void* payload;
    base::global::CStrId name;
    base::UnkVector<UnkNamedNode*> children;
};
static_assert(sizeof(UnkNamedNode) == 0x98, "UnkNamedNode placeholder layout");

UnkNamedNode* fn_7100195310(UnkNamedNode* node) {
    for (UnkNamedNode **p = node->children.data, **end = p + node->children.count; p != end; ++p) {
        UnkNamedNode* child = *p;
        if (child->payload)
            return child;
        if (UnkNamedNode* found = fn_7100195310(child))
            return found;
    }
    return nullptr;
}
