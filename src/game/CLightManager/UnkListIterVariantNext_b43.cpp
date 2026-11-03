// More copies of fn_710023a094 (src/game/CActor/UnkListIterVariantNext_b42.cpp), advancing an iterator over a
// linked list whose current element is kept in a two-state variant, each with its own unit's destroy
// (two in CLightManager's unit, four in unit 2640 of tools/tu_map.py). Names and layouts are placeholders.
#include "types.h"

// Signatures as src/clones/ declares them.
void fn_7100e0ccf8(char*);
void fn_7100e0d204(char*);
void fn_7100ff4350(char*);
void fn_7100ff4844(char*);
void fn_7100ff7360(char*);
void fn_7100ff7828(char*);

// The element (and so the node's next pointer offset) is 0x98 bytes in CLightManager's copies and 0x38
// in the others.
template <u32 N>
struct UnkListNode_b43a3 {
    u8 unk_0x0[N];
    UnkListNode_b43a3* next;
};

template <void (*Destroy)(char*), u32 N>
struct UnkListVariant_b43a3 {
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
struct UnkListIter_b43a3 {
    UnkListNode_b43a3<N>* node;
    UnkListVariant_b43a3<Destroy, N> variant;

    __attribute__((always_inline)) void Next() {
        UnkListNode_b43a3<N>* n = node->next;
        node = n;
        if (n)
            variant.SetPair(n, reinterpret_cast<u8*>(n) + 8);
        else
            variant.Reset();
    }
};

void fn_7100e0ca94(UnkListIter_b43a3<fn_7100e0ccf8, 0x98>* self) { self->Next(); }
void fn_7100e0cfa0(UnkListIter_b43a3<fn_7100e0d204, 0x98>* self) { self->Next(); }
void fn_7100ff40ec(UnkListIter_b43a3<fn_7100ff4350, 0x38>* self) { self->Next(); }
void fn_7100ff45e0(UnkListIter_b43a3<fn_7100ff4844, 0x38>* self) { self->Next(); }
void fn_7100ff70fc(UnkListIter_b43a3<fn_7100ff7360, 0x38>* self) { self->Next(); }
void fn_7100ff75c4(UnkListIter_b43a3<fn_7100ff7828, 0x38>* self) { self->Next(); }
