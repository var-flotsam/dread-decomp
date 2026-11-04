// Begin and end over arrays of 0x50-byte elements with the iterators of UnkUnit2640_IdxIter_b56.h: a
// temporary (index, element) iterator is synced out of line (in another file, so the temporary
// escapes), copied to the output unless it is the output, the output synced, and the temporary
// reset. Names and layouts are placeholders.
#include "UnkUnit2640_IdxIter_b56.h"

struct UnkElem50_b56 {
    u8 unk_0x0[0x50];
};

struct UnkElem50Array_b56 {
    UnkElem50_b56* data;
    u32 count;
};

__attribute__((always_inline)) inline void UnkRecordsIdxIterAt_b56(u32 i, UnkElem50_b56* c,
                                                                  UnkRecordsIdxIter_b56<0>* out) {
    UnkRecordsIdxIter_b56<0> it;
    it.index = i;
    it.current = reinterpret_cast<u8*>(c);
    it.state = 0;
    fn_7100ff1534(&it);
    if (out != &it) {
        out->index = it.index;
        out->current = it.current;
        fn_7100ff1534(out);
    }
    it.Reset();
}

void fn_7100ffdd34(UnkElem50Array_b56* self, UnkRecordsIdxIter_b56<0>* out) {
    UnkRecordsIdxIterAt_b56(0, self->data, out);
}

void fn_7100ffde5c(UnkElem50Array_b56* self, UnkRecordsIdxIter_b56<0>* out) {
    UnkRecordsIdxIterAt_b56(self->count, self->data + self->count, out);
}
