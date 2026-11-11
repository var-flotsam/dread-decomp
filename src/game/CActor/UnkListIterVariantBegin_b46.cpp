// Near copy of fn_710023a094 (src/game/CActor/UnkListIterVariantNext_b42.cpp) in CActor's unit:
// starting an iterator (at the owner's list head, +0x40) over a linked list whose current element is kept in a two-state variant
// (state 1 an object destroyed by fn_7100239e0c, state 2 a pair of pointers). Names and layouts are
// placeholders.
#include "game/UnkTU341.h"
#include "types.h"

// Signature as src/clones/clones_00.cpp declares it.

template <u32 N>
struct UnkListNode_b46a1 {
    u8 unk_0x0[N];
    UnkListNode_b46a1* next;
};

template <void (*Destroy)(char*), u32 N>
struct UnkListVariant_b46a1 {
    union {
        char object[N];
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            Destroy(object);
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    __attribute__((always_inline)) void SetPair(void* first, void* second) {
        s32* p = &state;
        switch (*p) {
        case 1:
            Destroy(object);
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
        state = 2;
        pair.first = first;
        pair.second = second;
    }
};

template <void (*Destroy)(char*), u32 N>
struct UnkListIter_b46a1 {
    UnkListNode_b46a1<N>* node;
    UnkListVariant_b46a1<Destroy, N> variant;

    __attribute__((always_inline)) void Begin(UnkListNode_b46a1<N>* n) {
        node = n;
        if (n)
            variant.SetPair(n, reinterpret_cast<u8*>(n) + 8);
        else
            variant.Reset();
    }
};

struct UnkListOwner_b46a1 {
    u8 unk_0x0[0x40];
    UnkListNode_b46a1<0x30>* head;
};

void fn_71002454ac(UnkListOwner_b46a1* owner, UnkListIter_b46a1<fn_7100239e0c, 0x30>* it) { it->Begin(owner->head); }
