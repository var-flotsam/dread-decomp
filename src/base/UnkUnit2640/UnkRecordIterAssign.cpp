// From unit 2640 of tools/tu_map.py.
// Shaped as fn_710011e718 (src/base/UnkUnit108/UnkPartsIterAssign.cpp): the copy assignment of a
// reflected iterator value (base/UnkCollectionIter.h) with a u32 index and a 0x50-byte record: a
// word, a part assigned by fn_7100ffbad8, then plain fields (an implicit copy assignment). Names
// and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "types.h"

struct UnkPartFfca00;
UnkPartFfca00* fn_7100ffbad8(UnkPartFfca00* self, const UnkPartFfca00* other);

struct UnkPartFfca00 {
    u64 unk_0x0[5];
    UnkPartFfca00& operator=(const UnkPartFfca00& other) {
        fn_7100ffbad8(this, &other);
        return *this;
    }
};

struct UnkRecordFfca00 {
    u32 field_0x0;
    UnkPartFfca00 part_0x8;
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
};

void* fn_7100ffca00(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkRecordFfca00, u32>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
