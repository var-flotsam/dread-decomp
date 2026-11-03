// nn::atk unit (tools/tu_map.py unit 2772): the first word of the pair of the ring node that matches `key`
// (fn_710120b310, as in src/atk_id/nn_atk_b49a3.cpp), or null when no node matches or the key is null.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAtkKeyedNode_b49;

struct UnkAtkLink_b49a6 {
    UnkAtkLink_b49a6* prev;
    UnkAtkLink_b49a6* next;
};

struct UnkAtkNode_b49a6 : UnkAtkLink_b49a6 {
    u8 pad_0x10[0x8];
    u64 value;  // 0x18
};

struct UnkAtkKeyedList_b49a6 {
    u8 pad_0x0[0x10];
    UnkAtkLink_b49a6 ring;  // 0x10
};

bool fn_710120b310(UnkAtkKeyedNode_b49* node, const void* key);

u64 fn_7101211398(UnkAtkKeyedList_b49a6* self, const void* key) {
    if (!key)
        return 0;
    for (UnkAtkLink_b49a6* link = self->ring.next; &self->ring != link; link = link->next) {
        UnkAtkNode_b49a6* node = static_cast<UnkAtkNode_b49a6*>(link);
        if (fn_710120b310(reinterpret_cast<UnkAtkKeyedNode_b49*>(node), key))
            return node->value;
    }
    return 0;
}

