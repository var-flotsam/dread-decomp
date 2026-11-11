// Advancing an iterator over a linked list (next node at +0x30) whose current element is kept in a
// two-state variant, one copy per element type (both in CActor's unit): the iterator moves to the
// next node; the variant is reset (state 1 holds an object destroyed out of line, state 2 a pair
// of pointers) and, when there is a node, takes state 2 with the node and its payload at +8.
// Reset is read twice as in src/base/animation/CBlendSpaceResource/UnkVariantHolderSet_b42.cpp (the
// original does not merge the two reads of the state). Names and layouts are placeholders.
#include "game/UnkTU341.h"
#include "types.h"

// Signatures as src/clones/clones_00.cpp declares them.
void fn_710023a2f8(char*);

struct UnkListNode_b42a1 {
    u8 unk_0x0[0x30];
    UnkListNode_b42a1* next;
};

template <void (*Destroy)(char*)>
struct UnkListVariant_b42a1 {
    union {
        char object[0x30];
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

template <void (*Destroy)(char*)>
struct UnkListIter_b42a1 {
    UnkListNode_b42a1* node;
    UnkListVariant_b42a1<Destroy> variant;

    __attribute__((always_inline)) void Next() {
        UnkListNode_b42a1* n = node->next;
        node = n;
        if (n)
            variant.SetPair(n, reinterpret_cast<u8*>(n) + 8);
        else
            variant.Reset();
    }
};

void fn_7100239ba8(UnkListIter_b42a1<fn_7100239e0c>* self) { self->Next(); }
void fn_710023a094(UnkListIter_b42a1<fn_710023a2f8>* self) { self->Next(); }
