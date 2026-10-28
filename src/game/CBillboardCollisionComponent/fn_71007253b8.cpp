// The element at an index of an array of 0x30-byte records (three string ids and three
// 8-byte values), growing the array to cover the index first (UnkVector::GrowAt).
#include "game/UnkCollisionRecord.h"
#include "types.h"

// The original has GrowAt inlined; flatten keeps it from being emitted out of line.
__attribute__((flatten)) UnkCollisionRecord& fn_71007253b8(base::UnkVector<UnkCollisionRecord>* self, u32 index) {
    return self->GrowAt(index);
}
