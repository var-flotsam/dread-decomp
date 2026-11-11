// nn::atk (tools/tu_map.py, library unit): walks the intrusive list at +0x8 (link at +0x288 of each
// element) and runs fn_710122cf1c on every element whose state (+0x278) is 0; the next link is read
// first, so the call may unlink the element. Names and layout are placeholders.
#include "game/UnkTU2772.h"
#include "types.h"

struct UnkAtkLink_b48a3 {
    UnkAtkLink_b48a3* prev;
    UnkAtkLink_b48a3* next;
};

struct UnkAtkListed_b48a3 {
    u8 pad_0x0[0x278];
    s32 state;
    u8 pad_0x27c[0x288 - 0x27c];
    UnkAtkLink_b48a3 link;
};

// As src/atk_id/nn_atk.cpp declares it.
struct UnkAtkListOwner;

struct UnkAtkListOwner_b48a3 {
    u8 pad_0x0[0x8];
    UnkAtkLink_b48a3 head;
};

static inline UnkAtkListed_b48a3* UnkAtkFromLink_b48a3(UnkAtkLink_b48a3* link) {
    return reinterpret_cast<UnkAtkListed_b48a3*>(reinterpret_cast<char*>(link) - 0x288);
}

void fn_710122f334(UnkAtkListOwner_b48a3* self) {
    UnkAtkLink_b48a3* it = self->head.next;
    while (&self->head != it) {
        UnkAtkLink_b48a3* next = it->next;
        UnkAtkListed_b48a3* element = UnkAtkFromLink_b48a3(it);
        if (element->state == 0)
            fn_710122cf1c(reinterpret_cast<UnkAtkListOwner*>(element));
        it = next;
    }
}
