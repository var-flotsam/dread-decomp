// CTriggerComponent unit (tools/tu_map.py): Sync of two iterators whose current element is kept in a
// two-state variant (copies of src/base/UnkUnit2609/UnkUnit2609_OwnedIterSync_b55.cpp): with a current
// entry (+8) the variant becomes the pair {iterator, entry}, else it is reset. State 1 owns an object at
// +0x18 (fn_7100a632e8 destroys it, fn_7100083c00 frees it). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkTriggerOwned_b56;
void fn_7100a632e8(UnkTriggerOwned_b56* self);

struct UnkTriggerOwnedObject_b56 {
    u64 field_0x0;
    UnkTriggerOwned_b56* owned;

    __attribute__((always_inline)) void Destroy() {
        UnkTriggerOwned_b56* p = owned;
        owned = nullptr;
        if (p) {
            fn_7100a632e8(p);
            fn_7100083c00(p);
        }
    }
};

struct UnkTriggerOwnedVariant_b56 {
    union {
        UnkTriggerOwnedObject_b56 object;
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
struct UnkTriggerOwnedIter_b56 {
    void* field_0x0;
    void* current;
    UnkTriggerOwnedVariant_b56 variant;

    __attribute__((always_inline)) void Sync() {
        void* c = current;
        if (c)
            variant.SetPair(this, c);
        else
            variant.Reset();
    }
};

void fn_7100a62a68(UnkTriggerOwnedIter_b56<0>* self) { self->Sync(); }
void fn_7100a62e20(UnkTriggerOwnedIter_b56<1>* self) { self->Sync(); }
