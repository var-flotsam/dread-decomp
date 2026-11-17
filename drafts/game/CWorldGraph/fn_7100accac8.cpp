// CWorldGraph's unit (tools/tu_map.py unit 1627): links the graph's nodes (fn_7100accac8): each node whose
// neighbour ids outnumber its neighbour pointers gets a pointer to every node it names that it lacks.
// Names and layouts are placeholders.
#include <algorithm>

#include "base/global/CRntVector.h"
#include "../../game_s7/UnkIter.h"
#include "types.h"

using base::global::CRntVector;

struct UnkWorldGraphNode_laccac8 {
    u64 field_0x0;
    u64 id;  // 0x8
    u8 pad_0x10[0x10];
    CRntVector<UnkWorldGraphNode_laccac8*> neighbours;  // 0x20
    CRntVector<u64> neighbour_ids;  // 0x48
    u8 pad_0x70[0x10];
};
static_assert(sizeof(UnkWorldGraphNode_laccac8) == 0x80, "layout");

struct UnkWorldGraph_laccac8 {
    u8 pad_0x0[0x30];
    CRntVector<UnkWorldGraphNode_laccac8> nodes;  // 0x30
};

static inline bool Contains(const CRntVector<UnkWorldGraphNode_laccac8*>& list, UnkWorldGraphNode_laccac8* node) {
    for (u32 i = 0; i < list.count; ++i) {
        if (list.data[i] == node)
            return true;
    }
    return false;
}

void fn_7100accac8(UnkWorldGraph_laccac8* self) {
    for (UnkWorldGraphNode_laccac8& node : self->nodes) {
        if (node.neighbour_ids.count == node.neighbours.count)
            continue;
        for (u64& id : node.neighbour_ids) {
            UnkWorldGraphNode_laccac8* end = self->nodes.end();
            UnkWorldGraphNode_laccac8* found =
                std::find_if(UnkIter<UnkWorldGraphNode_laccac8>{self->nodes.begin()}, UnkIter<UnkWorldGraphNode_laccac8>{end},
                             [id](const UnkWorldGraphNode_laccac8& other) { return other.id == id; })
                    .p;
            if (found != end) {
                if (!Contains(node.neighbours, found))
                    node.neighbours.PushBack(found);
            }
        }
    }
}
