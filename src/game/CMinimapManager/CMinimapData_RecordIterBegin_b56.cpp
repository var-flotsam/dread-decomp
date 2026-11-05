// CMinimapData's unit (tools/tu_map.py unit 2563): begin and end over an array of 0x60-byte records
// (UnkRecord60 of src/game_aca57c/fn_7100e5493c.cpp) with base::UnkCollectionIter iterators: a
// temporary (index, element) iterator is built and synced inline, copied to the output unless it is
// the output, the output synced out of line (fn_7100e46b58), and the temporary's value reset (state
// 1 owns an index and a record holding two arrays). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkRecord60;  // src/game_aca57c/fn_7100e5493c.cpp
void fn_7100e46b58(base::UnkCollectionIter<UnkRecord60, u32>* it);

struct UnkRecord60Iter_b56 {
    u32 index;
    u8* element;
    union {
        struct {
            u32 index;
            base::UnkVector<u8> part;
            base::UnkVector<u16> values;
            u8 unk_0x58[0x10];
        } value;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            value.values.Free();
            value.part.Free();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    __attribute__((always_inline)) UnkRecord60Iter_b56(u32 i, u8* e) : index(i), element(e), state(0) {
        if (e) {
            Reset();
            state = 2;
            pair.first = this;
            pair.second = e;
        } else {
            Reset();
        }
    }
    __attribute__((always_inline)) ~UnkRecord60Iter_b56() { Reset(); }
};

struct UnkRecord60Bytes_b56 {
    u8 unk_0x0[0x60];
};

struct UnkRecord60Vec_b56 {
    UnkRecord60Bytes_b56* data;
    u32 count;
};

using UnkRecord60CollIter_b56 = base::UnkCollectionIter<UnkRecord60, u32>;

__attribute__((always_inline)) inline void UnkRecord60IterAt_b56(u32 i, u8* e, UnkRecord60CollIter_b56* out) {
    UnkRecord60Iter_b56 it(i, e);
    UnkRecord60Iter_b56* o = reinterpret_cast<UnkRecord60Iter_b56*>(out);
    if (o != &it) {
        o->index = it.index;
        o->element = it.element;
        (fn_7100e46b58)(out);  // parenthesised: no argument-dependent lookup on the incomplete record
    }
}

void fn_7100e53838(UnkRecord60Vec_b56* self, UnkRecord60CollIter_b56* out) { (UnkRecord60IterAt_b56)(0, reinterpret_cast<u8*>(self->data), out); }

void fn_7100e538fc(UnkRecord60Vec_b56* self, UnkRecord60CollIter_b56* out) {
    (UnkRecord60IterAt_b56)(self->count, reinterpret_cast<u8*>(self->data + self->count), out);
}
