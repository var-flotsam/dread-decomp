// Unit 2601 of tools/tu_map.py: the move constructor of a reflected iterator value
// (base::refl_IterValueMove, base/UnkCollectionIter.h) with a string id key and an element of two
// string ids, an array of 0x50-byte records (UnkUnit2601_VecCopy_b56.h), a word and a part copied by
// fn_7100f84868; none of them has a move constructor. Names and layouts are placeholders.
#include "UnkUnit2601_VecCopy_b56.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkPartF84868_b56;
void fn_7100f84868(UnkPartF84868_b56* self, const UnkPartF84868_b56* other);

struct UnkPartF84868_b56 {
    u64 unk_0x0[5];
    UnkPartF84868_b56(const UnkPartF84868_b56& other) { fn_7100f84868(this, &other); }
};

struct UnkRecordsEntry_b56 {
    base::global::CStrId id;
    UnkRecord50VecMember_b56 records;
    base::global::CStrId id2;
    u32 field_0x38;
    UnkPartF84868_b56 part;
};

// base::refl_IterValueMove written out, so that the constructor is inlined.
__attribute__((flatten)) void fn_7100f86d20(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkRecordsEntry_b56, base::UnkStrIdMember>;
    new (dst) V(static_cast<V&&>(*static_cast<V*>(const_cast<void*>(src))));
}
