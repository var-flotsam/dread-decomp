// Unit 212 of tools/tu_map.py (0x7100194a9c-0x710019545c, no reflected class): a search of a tree of
// named nodes (the layout CCutscene.cpp uses): the first child with a payload, depth first, or null.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkNamedNode.h"
#include "types.h"

static_assert(sizeof(UnkNamedNode) == 0x98, "UnkNamedNode placeholder layout");

UnkNamedNode* FindFirstDescendantWithPayload(UnkNamedNode* node) {
    for (UnkNamedNode **p = node->children.data, **end = p + node->children.count; p != end; ++p) {
        UnkNamedNode* child = *p;
        if (child->payload)
            return child;
        if (UnkNamedNode* found = FindFirstDescendantWithPayload(child))
            return found;
    }
    return nullptr;
}
