#include "game/UnkTU2772.h"
#include "types.h"

struct UnkAtkListLink_b0148 {
    UnkAtkListLink_b0148* prev;
    UnkAtkListLink_b0148* next;
};

struct UnkAtkEmitter_b0147 {
    u8 pad_0x0[0x278];
    u32 state;  // 0x278
    u8 pad_0x27c[0xc];
    UnkAtkListLink_b0148 link;  // 0x288
};

struct UnkAtkChannelOwner_b0148 {
    u8 pad_0x0[0x8];
    UnkAtkListLink_b0148 list;  // 0x8
};

static inline UnkAtkEmitter_b0147* ItemOf(UnkAtkListLink_b0148* link) {
    return reinterpret_cast<UnkAtkEmitter_b0147*>(reinterpret_cast<u8*>(link) - 0x288);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710122f3f0(UnkAtkChannelOwner_b0148* self) {
    UnkAtkListLink_b0148* head = &self->list;
    for (UnkAtkListLink_b0148* it = head->next; head != it;) {
        UnkAtkListLink_b0148* next = it->next;
        if (ItemOf(it)->state == 0)
            fn_710122d048(ItemOf(it));
        it = next;
    }
    for (UnkAtkListLink_b0148* it = head->next; head != it;) {
        UnkAtkListLink_b0148* next = it->next;
        if (ItemOf(it)->state == 0)
            fn_710122d3bc(ItemOf(it));
        it = next;
    }
}

#if 0
// o0319 alternative: walking the second pass through a different link type stops the empty-list jump threading;
// only the second loop's compare operand order still differs.
#include "types.h"

struct UnkAtkLink_o0319 {
    UnkAtkLink_o0319* prev;
    UnkAtkLink_o0319* next;
};

// The second pass walks the same ring through a second view of the links.
struct UnkAtkLinkB_o0319 {
    UnkAtkLinkB_o0319* prev;
    UnkAtkLinkB_o0319* next;
};

struct UnkAtkChannelItem_o0319 {
    u8 pad_0x0[0x278];
    u32 state;  // 0x278
    u8 pad_0x27c[0xc];
    UnkAtkLink_o0319 link;  // 0x288
};

struct UnkAtkChannelOwner_o0319 {
    u8 pad_0x0[0x8];
    union {
        UnkAtkLink_o0319 list;  // 0x8
        UnkAtkLinkB_o0319 list_b;
    };
};

struct UnkAtkEmitter_b0147;
void fn_710122f3f0(UnkAtkChannelOwner_o0319* self) {
    for (UnkAtkLink_o0319* it = self->list.next; &self->list != it;) {
        UnkAtkLink_o0319* next = it->next;
        UnkAtkChannelItem_o0319* item = reinterpret_cast<UnkAtkChannelItem_o0319*>(reinterpret_cast<u8*>(it) - 0x288);
        if (item->state == 0)
            fn_710122d048(reinterpret_cast<UnkAtkEmitter_b0147*>(item));
        it = next;
    }
    for (UnkAtkLinkB_o0319* it = self->list_b.next; &self->list_b != it;) {
        UnkAtkLinkB_o0319* next = it->next;
        UnkAtkChannelItem_o0319* item = reinterpret_cast<UnkAtkChannelItem_o0319*>(reinterpret_cast<u8*>(it) - 0x288);
        if (item->state == 0)
            fn_710122d3bc(reinterpret_cast<UnkAtkEmitter_b0147*>(item));
        it = next;
    }
}
#endif
