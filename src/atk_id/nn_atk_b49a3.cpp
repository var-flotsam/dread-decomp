// nn::atk unit (tools/tu_map.py unit 2772): optional parameters after a flag word at +0x14, one 4-byte
// slot for each low bit that is set, in bit order (as in src/atk_id/nn_atk_b34a3.cpp). fn_7101210178
// returns the block plus the offset in bit 8's slot, or null when bit 8 is clear. Names and layouts are
// placeholders.
#include "types.h"

struct UnkAtkOptionalParams_b49 {
    u32 flags;
    u32 slots[1];

    bool Has(u32 bit) const { return static_cast<u64>(flags) >> bit & 1; }
    const u32* Slot(u32 bit) const {
        u64 count = 0;
        for (u32 i = 0; i < bit; ++i)
            if (Has(i))
                ++count;
        return reinterpret_cast<const u32*>(this) + count + 1;
    }
};

struct UnkAtkOptionalBlock_b49 {
    u8 pad_0x0[0x14];
    UnkAtkOptionalParams_b49 params;  // 0x14
};

const char* fn_7101210178(const UnkAtkOptionalBlock_b49* self) {
    const UnkAtkOptionalParams_b49* params = &self->params;
    if (!params->Has(8))
        return nullptr;
    return reinterpret_cast<const char*>(self) + *params->Slot(8);
}

// A pair picked by key: the default pair (+0x0) unless a node of the ring at +0x10 (next at +0x8, pair at
// +0x18) matches `key` (fn_710120b310); the choice lands at +0x20. A null key keeps the default.
struct UnkAtkPair_b49 {
    u64 first;
    u64 second;
};

struct UnkAtkRingLink_b49 {
    UnkAtkRingLink_b49* prev;
    UnkAtkRingLink_b49* next;
};

struct UnkAtkKeyedNode_b49 : UnkAtkRingLink_b49 {
    u8 pad_0x10[0x8];
    UnkAtkPair_b49 pair;  // 0x18
};

struct UnkAtkPairPicker_b49 {
    UnkAtkPair_b49 fallback;
    UnkAtkRingLink_b49 ring;  // 0x10
    UnkAtkPair_b49 chosen;    // 0x20
};

bool fn_710120b310(UnkAtkKeyedNode_b49* node, const void* key);

void fn_7101211260(UnkAtkPairPicker_b49* self, const void* key) {
    self->chosen.first = self->fallback.first;
    self->chosen.second = self->fallback.second;
    if (!key)
        return;
    for (UnkAtkRingLink_b49* link = self->ring.next; &self->ring != link; link = link->next) {
        UnkAtkKeyedNode_b49* node = static_cast<UnkAtkKeyedNode_b49*>(link);
        if (fn_710120b310(node, key)) {
            self->chosen.first = node->pair.first;
            self->chosen.second = node->pair.second;
            return;
        }
    }
}
