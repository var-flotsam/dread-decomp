// From unit 108 of tools/tu_map.py: the copy assignment of a reflected iterator value
// (base/UnkCollectionIter.h) with a u32 index and a 0x48-byte record whose leading part is
// assigned by fn_710011dac0 after two values and a flag set on assignment, then one more value
// (the start of the UnkPartsRecord of src/base/reflection/fn_710025f31c.cpp). Names and layouts
// are placeholders.
#include "base/UnkCollectionIter.h"
#include "types.h"

struct UnkPartsRecord;
void fn_710011dac0(UnkPartsRecord* self, const UnkPartsRecord* other);

namespace {
struct UnkPartsHead {
    u64 unk_0x0[5];
    u32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    u64 unk_0x30[2];
    u32 field_0x40;

    UnkPartsHead& operator=(const UnkPartsHead& other) {
        field_0x28 = other.field_0x28;
        field_0x2c = other.field_0x2c;
        field_0x2d = true;
        fn_710011dac0(reinterpret_cast<UnkPartsRecord*>(this), reinterpret_cast<const UnkPartsRecord*>(&other));
        field_0x40 = other.field_0x40;
        return *this;
    }
};
static_assert(sizeof(UnkPartsHead) == 0x48, "UnkPartsHead layout");
}  // namespace

void* fn_710011e718(void* dst, const void* src) {
    using V = base::UnkIterValue<UnkPartsHead, u32>;
    return &(*static_cast<V*>(dst) = *static_cast<const V*>(src));
}
