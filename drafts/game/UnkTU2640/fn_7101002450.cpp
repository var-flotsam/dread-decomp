// Unit 2640: the reflected Begin of an array of 0x78-byte records that each hold an array of file-path
// records and an array of words: the iterator starts at the first record. Names and layouts are
// placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// A file-path id and 0x10 more bytes.
struct UnkTU2640PathRecord_l002450 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> id;  // CFilePathStrId's pool
    u8 unk_0x8[0x10];
};

struct UnkTU2640Record_l002450 {
    u64 field_0x0;
    base::global::CRntVector<UnkTU2640PathRecord_l002450> records;  // 0x8
    u8 unk_0x30[0x20];
    base::global::CRntVector<u64> words;  // 0x50

    ~UnkTU2640Record_l002450() {
        words.Free();
        records.Free();
    }
};
static_assert(sizeof(UnkTU2640Record_l002450) == 0x78, "UnkTU2640Record_l002450 layout");

void fn_7101002450(const base::global::CRntVector<UnkTU2640Record_l002450>* v,
                   base::UnkCollectionIter<UnkTU2640Record_l002450>* out) {
    base::refl_Begin(v, out);
}
