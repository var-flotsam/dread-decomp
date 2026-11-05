// CTriggerComponent unit (tools/tu_map.py): begin and end over an array of pointers (copies of
// src/base/UnkUnit2609/UnkUnit2609_OwnedIterBegin_b56.cpp). A temporary (index, element) iterator is
// built and synced inline, copied to the output unless it is the output, the output synced out of line
// (fn_7100a62a68, src/game/CTriggerComponent/CTriggerComponent_OwnedIterSync_b56.cpp) and the
// temporary's variant reset (state 1 owns an object: fn_7100a632e8 destroys it, fn_7100083c00 frees
// it). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

template <int Tag>
struct UnkTriggerOwnedIter_b56;
void fn_7100a62a68(UnkTriggerOwnedIter_b56<0>* self);

struct UnkTriggerOwned_b56;
void fn_7100a632e8(UnkTriggerOwned_b56* self);

struct UnkTriggerIdxIter_b59 {
    u32 index;
    void** current;
    union {
        struct {
            u64 field_0x0;
            UnkTriggerOwned_b56* owned;
        } object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1: {
            UnkTriggerOwned_b56* p = object.owned;
            object.owned = nullptr;
            if (p) {
                fn_7100a632e8(p);
                fn_7100083c00(p);
            }
            state = 0;
            break;
        }
        case 2:
            state = 0;
            break;
        }
    }

    __attribute__((always_inline)) UnkTriggerIdxIter_b59(u32 i, void** c) : index(i), current(c), state(0) {
        if (c) {
            Reset();
            state = 2;
            pair.first = this;
            pair.second = c;
        } else {
            Reset();
        }
    }
    __attribute__((always_inline)) ~UnkTriggerIdxIter_b59() { Reset(); }
};

struct UnkTriggerPtrArray_b59 {
    void** data;
    u32 count;
};

__attribute__((always_inline)) inline void UnkTriggerIdxIterAt_b59(u32 i, void** c, UnkTriggerOwnedIter_b56<0>* out) {
    UnkTriggerIdxIter_b59 it(i, c);
    UnkTriggerIdxIter_b59* o = reinterpret_cast<UnkTriggerIdxIter_b59*>(out);
    if (o != &it) {
        o->index = it.index;
        o->current = it.current;
        fn_7100a62a68(out);
    }
}

void fn_7100a64328(UnkTriggerPtrArray_b59* self, UnkTriggerOwnedIter_b56<0>* out) {
    UnkTriggerIdxIterAt_b59(0, self->data, out);
}

void fn_7100a643d0(UnkTriggerPtrArray_b59* self, UnkTriggerOwnedIter_b56<0>* out) {
    UnkTriggerIdxIterAt_b59(self->count, self->data + self->count, out);
}
