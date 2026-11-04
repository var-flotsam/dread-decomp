// Iterators over linked lists whose current element is kept in a two-state variant (the shape of
// src/game/CLightManager/UnkListIterVariantNext_b43.cpp): Sync sets the variant from the current
// node, as a pair {node, node + 8}, or resets it at the end. State 1 holds a {string id, array}
// object, destroyed inline. Two in CBTObserverComponent's unit (tools/tu_map.py unit 2216) and one
// in CSubAreaManager's (unit 2577). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

template <int Tag>
struct UnkIdArrayObject_b55 {
    base::UnkStrIdMember id;
    base::UnkVector<u8> items;

    __attribute__((always_inline)) void Destroy() {
        items.Free();
        id.~UnkStrIdMemberOf();
    }
};

template <int Tag>
struct UnkListVariant_b55 {
    union {
        UnkIdArrayObject_b55<Tag> object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            object.Destroy();
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
            object.Destroy();
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

template <int Tag>
struct UnkListIter_b55 {
    u8* node;
    UnkListVariant_b55<Tag> variant;

    __attribute__((always_inline)) void Sync() {
        u8* n = node;
        if (n)
            variant.SetPair(n, n + 8);
        else
            variant.Reset();
    }
};

void fn_7100ca8b88(UnkListIter_b55<0>* self) { self->Sync(); }
void fn_7100ca9004(UnkListIter_b55<1>* self) { self->Sync(); }
void fn_7100ef1abc(UnkListIter_b55<2>* self) { self->Sync(); }
