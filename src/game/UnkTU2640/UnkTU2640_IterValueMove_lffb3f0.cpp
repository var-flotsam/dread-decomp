// Unit 2640: the move helper of the iterator value over a reflected array of 0x18-byte elements that begin with a
// file-path id (copied, its count raised). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

// A word and a half-word, copied as one 12-byte block (packed: the copy skips the tail padding).
struct __attribute__((packed)) UnkTU2640PathData {
    u64 field_0x0;
    u32 field_0x8;
};

struct UnkTU2640PathElem {
    base::global::CFilePathStrId path;
    UnkTU2640PathData data;  // 0x8

    UnkTU2640PathElem(UnkTU2640PathElem&& other) : path(other.path), data(other.data) {}
};

void fn_7100ffb3f0(void* dst, const void* src) {
    base::refl_IterValueMove<UnkTU2640PathElem>(dst, src);
}
