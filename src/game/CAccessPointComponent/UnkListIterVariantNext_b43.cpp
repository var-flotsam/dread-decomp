// CAccessPointComponent unit (tools/tu_map.py): two copies of fn_710023a094
// (src/game/CActor/UnkListIterVariantNext_b42.cpp), advancing an iterator over a linked list (next
// node at +0x30) whose current element is kept in a two-state variant: the iterator moves to the next
// node; the variant is reset (state 1 holds an object destroyed out of line, state 2 a pair of
// pointers) and, when there is a node, takes state 2 with the node and its payload at +8. Names and
// layouts are placeholders.
#include "types.h"

// Signatures as src/game/CChangeStageNavMeshItemComponent/fn_71009ba4ac.cpp defines them.
struct UnkNamedIds;
void fn_7100a72af4(UnkNamedIds* self);
void fn_7100a73894(UnkNamedIds* self);

struct UnkListNode_b43a2 {
    u8 unk_0x0[0x30];
    UnkListNode_b43a2* next;
};

template <void (*Destroy)(UnkNamedIds*)>
struct UnkListVariant_b43a2 {
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
            Destroy(reinterpret_cast<UnkNamedIds*>(object));
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
            Destroy(reinterpret_cast<UnkNamedIds*>(object));
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

template <void (*Destroy)(UnkNamedIds*)>
struct UnkListIter_b43a2 {
    UnkListNode_b43a2* node;
    UnkListVariant_b43a2<Destroy> variant;

    __attribute__((always_inline)) void Next() {
        UnkListNode_b43a2* n = node->next;
        node = n;
        if (n)
            variant.SetPair(n, reinterpret_cast<u8*>(n) + 8);
        else
            variant.Reset();
    }
};

void fn_7100a73220(UnkListIter_b43a2<fn_7100a72af4>* self) { self->Next(); }
void fn_7100a73630(UnkListIter_b43a2<fn_7100a73894>* self) { self->Next(); }
