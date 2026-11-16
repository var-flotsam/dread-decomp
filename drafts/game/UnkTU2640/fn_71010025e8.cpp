// Unit 2640: the reflected End of an array of 0x78-byte records that each hold an array of file-path
// records and an array of words: the iterator is placed past the last record. Names and layouts are
// placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// A file-path id and 0x10 more bytes.
struct UnkTU2640PathRecord_l0025e8 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> id;  // CFilePathStrId's pool
    u8 unk_0x8[0x10];
};

struct UnkTU2640Record_l0025e8 {
    u64 field_0x0;
    base::global::CRntVector<UnkTU2640PathRecord_l0025e8> records;  // 0x8
    u8 unk_0x30[0x20];
    base::global::CRntVector<u64> words;  // 0x50

    ~UnkTU2640Record_l0025e8() {
        words.Free();
        records.Free();
    }
};
static_assert(sizeof(UnkTU2640Record_l0025e8) == 0x78, "UnkTU2640Record_l0025e8 layout");

using UnkTU2640RecordIter_l0025e8 = base::UnkCollectionIter<UnkTU2640Record_l0025e8>;

void fn_71010025e8(const base::global::CRntVector<UnkTU2640Record_l0025e8>* v, UnkTU2640RecordIter_l0025e8* out) {
    *out = UnkTU2640RecordIter_l0025e8(v->count, v->data + v->count);
}
