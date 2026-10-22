// Copies of fn_7100d44810 and fn_7100d4492c (src/game_aca57c/fn_7100d40ba0.cpp) for another element type, naming their
// own callees and data (the comments are adapted from the original's): fn_710011fbf0, fn_710011fd0c. The reflected
// collection functions Begin and End (base::refl_Begin/refl_End) of an array of records holding a vector of plain
// values (an owned vector is freed) and 0x20 more bytes.
#include "base/UnkCollectionIter.h"
#include "types.h"

struct UnkPlainVector : base::UnkVector<u32> {
    ~UnkPlainVector() { Free(); }
};

struct UnkPlainVectorRecord {
    UnkPlainVector vector;
    u64 unk_0x28[4];
};

using UnkPlainVectorRecordIter = base::UnkCollectionIter<UnkPlainVectorRecord>;

void fn_710011fbf0(const base::UnkVector<UnkPlainVectorRecord>* v, UnkPlainVectorRecordIter* out) {
    *out = UnkPlainVectorRecordIter(0, v->data);
}
void fn_710011fd0c(const base::UnkVector<UnkPlainVectorRecord>* v, UnkPlainVectorRecordIter* out) {
    *out = UnkPlainVectorRecordIter(v->count, v->data + v->count);
}
