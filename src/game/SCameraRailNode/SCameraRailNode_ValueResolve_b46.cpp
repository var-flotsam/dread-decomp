// SCameraRailNode's unit (tools/tu_map.py unit 1934): resolves a track value holding a word and a
// camera-rail node (src/game/SCameraRailNode/SCameraRailNode_VectorCopy_b43.cpp): a stored value
// (kind 1) is moved over, a value given through two pointers (kind 2) is moved from what they point
// at (the node: its vtable, three words and its callback set, moved with fn_71002458a4); the result
// is stored (kind 1) or left empty (kind 0). The shape of fn_7100b179f8 (GUI track values). Names
// and layouts are placeholders.
#include <new>

#include "game/SCameraRailNode.h"
#include "types.h"

// The move constructor, typed as CCentralUnitComponent's 0x40-byte half (one declaration per symbol).
struct UnkHalf_cu {
    u8 data[0x40];
    UnkHalf_cu(UnkHalf_cu&& other) asm("fn_71002458a4");
};

struct UnkRailNodeRaw_b46a3 {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 pad_0x14;
    UnkHalf_cu set;  // 0x18
};

struct UnkRailValue_b46a3 {
    u32 word;
    u32 pad_0x4;
    UnkRailNodeRaw_b46a3 node;  // 0x8
};

struct UnkRailRefs_b46a3 {
    u32* word;
    UnkRailNodeRaw_b46a3* node;
};

struct UnkRailTrackValue_b46a3 {
    union {
        UnkRailValue_b46a3 value;
        UnkRailRefs_b46a3 refs;
    };
    u32 kind;  // 0x60
};

static inline void MoveNode(UnkRailNodeRaw_b46a3* d, UnkRailNodeRaw_b46a3* s) {
    d->vtable = g_unk_7101aa2888 + 0x10;
    d->field_0x8 = s->field_0x8;
    d->field_0xc = s->field_0xc;
    d->field_0x10 = s->field_0x10;
    new (&d->set) UnkHalf_cu(static_cast<UnkHalf_cu&&>(s->set));
}

void fn_7100c081d0(UnkRailTrackValue_b46a3* out, UnkRailTrackValue_b46a3* in) {
    out->kind = 0;
    switch (in->kind) {
    case 1:
        out->kind = 1;
        out->value.word = in->value.word;
        MoveNode(&out->value.node, &in->value.node);
        break;
    case 2: {
        out->kind = 1;
        UnkRailRefs_b46a3 refs = in->refs;
        out->value.word = *refs.word;
        MoveNode(&out->value.node, refs.node);
        break;
    }
    }
}
