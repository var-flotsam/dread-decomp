// CEmmyAIComponent's unit (tools/tu_map.py unit 811): the destructor of a record of a word and three node
// owners (base/UnkNodeOwner.h; next link at +8, nothing to destroy per node) at +8, +0x78 and +0xe8, whose
// vtables are 0x7101a48be0/0x7101a21030, 0x7101a48c00/0x7101a1bdf8 and 0x7101a48be0/0x7101a21030.
// Names are placeholders.
#include "base/UnkNodeOwner.h"
#include "types.h"

extern void* g_unk_7101a48be0[];
extern void* g_unk_7101a21030[];
extern void* g_unk_7101a48c00[];
extern void* g_unk_7101a1bdf8[];

__attribute__((always_inline)) inline void UnkEmmyNoNodeDtor_A3_b69(UnkListNode<8>*) {}

using UnkEmmyNodes_A3_b69 = UnkNodeOwner<UnkListNode<8>, UnkEmmyNoNodeDtor_A3_b69>;

struct UnkEmmyNodeRecord_A3_b69 {
    u64 field_0x0;
    UnkEmmyNodes_A3_b69 first;
    UnkEmmyNodes_A3_b69 second;
    UnkEmmyNodes_A3_b69 third;
};
static_assert(sizeof(UnkEmmyNodeRecord_A3_b69) == 0x158, "UnkEmmyNodeRecord_A3_b69 layout");

void fn_710053ff9c(UnkEmmyNodeRecord_A3_b69* self) {
    self->third.Destroy(g_unk_7101a48be0, g_unk_7101a21030);
    self->second.Destroy(g_unk_7101a48c00, g_unk_7101a1bdf8);
    self->first.Destroy(g_unk_7101a48be0, g_unk_7101a21030);
}
