// Translation unit 958 (tools/tu_map.py, no class name; the attack-offset registry): the actor of the
// list at +0x40 nearest to `ref` (squared distance of the transform translations at +0x44/+0x54/+0x64),
// optionally only among actors whose AI component (+0x240) has its byte at +0x1220 set; null when the
// list is empty. Names and layouts are placeholders.
#include "types.h"

struct UnkClosestAI_b67a4 {
    u8 pad_0x0[0x1220];
    bool field_0x1220;
};

struct UnkClosestActor_b67a4 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x240 - 0x68];
    UnkClosestAI_b67a4* ai;  // 0x240
};

struct UnkClosestEntry_b67a4 {
    u8 pad_0x0[0x10];
    UnkClosestActor_b67a4* actor;  // 0x10
};

struct UnkClosestNode_b67a4 {
    UnkClosestEntry_b67a4* entry;
    UnkClosestNode_b67a4* next;
};

struct UnkClosestRegistry_b67a4 {
    u8 pad_0x0[0x40];
    UnkClosestNode_b67a4* list;  // 0x40
};

UnkClosestActor_b67a4* fn_710063c1f4(UnkClosestRegistry_b67a4* self, const UnkClosestActor_b67a4* ref,
                                     bool onlyFlagged) {
    UnkClosestNode_b67a4* n = self->list;
    if (!n)
        return nullptr;
    f32 x = ref->x;
    f32 y = ref->y;
    f32 z = ref->z;
    f32 best = 3.4028235e38f;
    UnkClosestActor_b67a4* result = nullptr;
    do {
        UnkClosestActor_b67a4* a = n->entry->actor;
        n = n->next;
        f32 ax = a->x;
        f32 ay = a->y;
        f32 az = a->z;
        f32 d = (x - ax) * (x - ax) + (y - ay) * (y - ay) + (z - az) * (z - az);
        bool ok = !onlyFlagged || a->ai->field_0x1220;
        bool closer = d < best;
        bool take = closer && ok;
        best = take ? d : best;
        result = take ? a : result;
    } while (n);
    return result;
}
