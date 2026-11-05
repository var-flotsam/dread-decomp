// Begin and end over an array of pointers, with the iterators src/base/UnkUnit2609/
// UnkUnit2609_OwnedIterSync_b55.cpp syncs (fn_7100f9f5d0): a temporary (index, element) iterator is
// built and synced inline, copied to the output unless it is the output, the output synced out of
// line, and the temporary's variant reset (state 1 owns an object, fn_7100ef1e1c destroys it and
// fn_7100083c00 frees it). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

template <int Tag>
struct UnkOwnedIter_b55;
void fn_7100f9f5d0(UnkOwnedIter_b55<0>* self);

struct UnkNamedIds;  // src/game/CSubAreaManager.cpp
void fn_7100ef1e1c(UnkNamedIds* self);

struct UnkOwnedIdxIter_b56 {
    u32 index;
    void** current;
    union {
        struct {
            u64 field_0x0;
            UnkNamedIds* owned;
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
            UnkNamedIds* p = object.owned;
            object.owned = nullptr;
            if (p) {
                fn_7100ef1e1c(p);
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

    __attribute__((always_inline)) UnkOwnedIdxIter_b56(u32 i, void** c) : index(i), current(c), state(0) {
        if (c) {
            Reset();
            state = 2;
            pair.first = this;
            pair.second = c;
        } else {
            Reset();
        }
    }
    __attribute__((always_inline)) ~UnkOwnedIdxIter_b56() { Reset(); }
};

struct UnkPtrArray_b56 {
    void** data;
    u32 count;
};

__attribute__((always_inline)) inline void UnkOwnedIdxIterAt_b56(u32 i, void** c, UnkOwnedIter_b55<0>* out) {
    UnkOwnedIdxIter_b56 it(i, c);
    UnkOwnedIdxIter_b56* o = reinterpret_cast<UnkOwnedIdxIter_b56*>(out);
    if (o != &it) {
        o->index = it.index;
        o->current = it.current;
        fn_7100f9f5d0(out);
    }
}

void fn_7100fa0cc8(UnkPtrArray_b56* self, UnkOwnedIter_b55<0>* out) {
    UnkOwnedIdxIterAt_b56(0, self->data, out);
}

void fn_7100fa0d70(UnkPtrArray_b56* self, UnkOwnedIter_b55<0>* out) {
    UnkOwnedIdxIterAt_b56(self->count, self->data + self->count, out);
}
