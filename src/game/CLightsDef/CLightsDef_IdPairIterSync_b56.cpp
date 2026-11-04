// CLightsDef's unit (tools/tu_map.py unit 2529): Sync of (index, element) iterators whose current
// element is kept in a two-state variant (the shape of
// src/game/CLightManager/UnkListIterVariantNext_b43.cpp): with an element the variant becomes the pair
// {iterator, element}, else it is reset. State 1 holds an object with two string ids at +0x10 and
// +0x18, released in reverse. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "types.h"

template <int Tag>
struct UnkIdPairIdxIter_b56 {
    u32 index;
    u8* current;
    union {
        struct {
            u8 unk_0x0[0x10];
            base::UnkStrIdMember first;
            base::UnkStrIdMember second;
            u8 unk_0x20[0x48];
        } object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    __attribute__((always_inline)) void DestroyObject() {
        object.second.~UnkStrIdMemberOf();
        object.first.~UnkStrIdMemberOf();
    }

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            DestroyObject();
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
            DestroyObject();
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

    __attribute__((always_inline)) void Sync() {
        u8* c = current;
        if (c)
            SetPair(this, c);
        else
            Reset();
    }
};

void fn_7100d9fce8(UnkIdPairIdxIter_b56<0>* self) { self->Sync(); }
void fn_7100da0208(UnkIdPairIdxIter_b56<1>* self) { self->Sync(); }
