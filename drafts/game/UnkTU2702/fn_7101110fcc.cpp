// Unit 2702 (hint CAIComponent): for event kind 2 or 4, caches a value read through the owner's
// target chain together with a position of its second object. Names and layouts are placeholders.
#include "types.h"

struct UnkVec3_l110fcc {
    f32 x, y, z;
};

struct UnkValueObj_l110fcc {
    u8 pad_0x0[0xac];
    u32 field_0xac;
};

struct UnkKindObj_l110fcc {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

struct UnkLink_l110fcc {
    u8 pad_0x0[0x10];
    UnkKindObj_l110fcc* field_0x10;
};

struct UnkChain_l110fcc {
    UnkLink_l110fcc* first;  // 0x0
    u8 pad_0x8[8];
    UnkValueObj_l110fcc* value2;  // 0x10
    u8 pad_0x18[8];
    UnkValueObj_l110fcc* value4;  // 0x20
};

struct UnkTargetA_l110fcc {
    u8 pad_0x0[0xb0];
    UnkChain_l110fcc* chain;  // 0xb0
};

struct UnkTargetB_l110fcc {
    u8 pad_0x0[0x40];
    UnkVec3_l110fcc position;  // 0x40
};

struct UnkCacheOwner_l110fcc {
    u8 pad_0x0[0x1f0];
    UnkTargetB_l110fcc* field_0x1f0;
    u8 pad_0x1f8[0xa0];
    UnkTargetA_l110fcc* field_0x298;
};

struct UnkCache_l110fcc {
    u32 value;
    UnkVec3_l110fcc position;
};

struct UnkCacheSelf_l110fcc {
    u8 pad_0x0[0x30];
    UnkCacheOwner_l110fcc* owner;  // 0x30
    u8 pad_0x38[0x14640];
    UnkCache_l110fcc cache;  // 0x14678
};

void fn_7101110fcc(UnkCacheSelf_l110fcc* self, u32 kind) {
    if (kind != 4 && kind != 2)
        return;
    UnkCacheOwner_l110fcc* owner = self->owner;
    UnkTargetA_l110fcc* a = owner->field_0x298;
    UnkTargetB_l110fcc* b = owner->field_0x1f0;
    if (a == nullptr || b == nullptr)
        return;
    UnkChain_l110fcc* chain = a->chain;
    UnkLink_l110fcc* first = chain->first;
    if (first == nullptr)
        return;
    UnkKindObj_l110fcc* node = first->field_0x10;
    if (node == nullptr)
        return;
    if (node->kind == 2 && chain->value2 != nullptr) {
        self->cache.value = chain->value2->field_0xac;
        self->cache.position = b->position;
        return;
    }
    if (node->kind == 4 && chain->value4 != nullptr) {
        self->cache.value = chain->value4->field_0xac;
        self->cache.position = b->position;
    }
}
