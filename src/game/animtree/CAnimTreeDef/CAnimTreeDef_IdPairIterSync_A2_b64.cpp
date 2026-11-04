// animtree::CAnimTreeDef's unit: copies of the (index, element) iterator Sync in
// src/game/CLightsDef/CLightsDef_IdPairIterSync_b56.cpp (fn_7100da0208). With an element the
// two-state variant becomes the pair {iterator, element}, else it is reset. State 1 holds an
// object with two string ids at +0x8 and +0x10. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "types.h"

template <int Tag>
struct UnkAnimTreeIdPairIdxIter_b64a2 {
    u32 index;
    u8* current;
    union {
        struct {
            u8 unk_0x0[0x8];
            base::UnkStrIdMember first;
            base::UnkStrIdMember second;
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

// Declared with this signature in src/game/animtree/CAnimTreeDef/CAnimTreeDef_IterBeginEnd_b49.cpp.
struct UnkAnimTreeIdPairIter_b49a1;
void fn_71002d8bac(UnkAnimTreeIdPairIter_b49a1* it) {
    reinterpret_cast<UnkAnimTreeIdPairIdxIter_b64a2<0>*>(it)->Sync();
}

void fn_71002d90cc(UnkAnimTreeIdPairIdxIter_b64a2<1>* self) { self->Sync(); }
