#include "game/UnkTU2772.h"
#include "types.h"

struct UnkAtkLink_b0147 {
    UnkAtkLink_b0147* prev;
    UnkAtkLink_b0147* next;
};

struct UnkAtkEmitterSet_b0147 {
    u8 pad_0x0[8];
    UnkAtkLink_b0147 list;  // 0x8
};

static inline UnkAtkEmitter_b0147* EmitterOf(UnkAtkLink_b0147* link) {
    return reinterpret_cast<UnkAtkEmitter_b0147*>(reinterpret_cast<u8*>(link) - 0x288);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710122f380(UnkAtkEmitterSet_b0147* self) {
    for (UnkAtkLink_b0147* n = self->list.next; &self->list != n;) {
        UnkAtkLink_b0147* next = n->next;
        fn_710122d048(EmitterOf(n));
        n = next;
    }
    for (UnkAtkLink_b0147* n = self->list.next; &self->list != n;) {
        UnkAtkLink_b0147* next = n->next;
        fn_710122d3bc(EmitterOf(n));
        n = next;
    }
}

#if 0
// o0319 alternative: walking the second pass through a different link type stops the empty-list jump threading;
// only the second loop's compare operand order still differs.
#include "game/UnkTU2772.h"
#include "types.h"

struct UnkAtkLink_o0319 {
    UnkAtkLink_o0319* prev;
    UnkAtkLink_o0319* next;
};

// The second pass walks the same ring through an untyped view of the links.
struct UnkAtkRawLink_o0319 {
    u8* prev;
    u8* next;
};

struct UnkAtkEmitterSet_o0319 {
    u8 pad_0x0[8];
    union {
        UnkAtkLink_o0319 list;  // 0x8
        UnkAtkRawLink_o0319 raw;
    };
};

void fn_710122f380(UnkAtkEmitterSet_o0319* self) {
    for (UnkAtkLink_o0319* n = self->list.next; &self->list != n;) {
        UnkAtkLink_o0319* next = n->next;
        fn_710122d048(reinterpret_cast<UnkAtkEmitter_b0147*>(reinterpret_cast<u8*>(n) - 0x288));
        n = next;
    }
    u8* head = reinterpret_cast<u8*>(&self->raw);
    for (u8* n = self->raw.next; head != n;) {
        u8* next = reinterpret_cast<UnkAtkRawLink_o0319*>(n)->next;
        fn_710122d3bc(reinterpret_cast<UnkAtkEmitter_b0147*>(n - 0x288));
        n = next;
    }
}
#endif
