// CLightsDef's unit (tools/tu_map.py unit 2529): Begin and End (slots 0x160 and 0x168) of a reflected
// array of 0x60-byte entries holding two string ids (+0x8, +0x10), the shape of animtree::CAnimTreeDef's
// (src/game/animtree/CAnimTreeDef/CAnimTreeDef_IterBeginEnd_b49.cpp). The iterator's constructor and
// assignment re-point its cached value through fn_7100d9fce8, kept out of line. Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkLightsIdPair_b54 {
    u64 field_0x0;
    base::UnkStrIdMember first;
    base::UnkStrIdMember second;
    u8 unk_0x18[0x48];
};
static_assert(sizeof(UnkLightsIdPair_b54) == 0x60, "layout");

struct UnkLightsIdPairArray_b54 {
    UnkLightsIdPair_b54* data;
    u32 count;
};

struct UnkLightsIdPairIter_b54;
void fn_7100d9fce8(UnkLightsIdPairIter_b54* it);  // re-points `current` at (index, element)

struct UnkLightsIdPairIter_b54 {
    u32 index;
    UnkLightsIdPair_b54* element;
    base::UnkIterValue<UnkLightsIdPair_b54, u32> current;

    UnkLightsIdPairIter_b54(u32 index, UnkLightsIdPair_b54* element) : index(index), element(element) {
        fn_7100d9fce8(this);
    }
    UnkLightsIdPairIter_b54& operator=(const UnkLightsIdPairIter_b54& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_7100d9fce8(this);
        }
        return *this;
    }
};

void fn_7100da1ab0(UnkLightsIdPairArray_b54* self, UnkLightsIdPairIter_b54* out) {
    *out = UnkLightsIdPairIter_b54(0, self->data);
}

void fn_7100da1bbc(UnkLightsIdPairArray_b54* self, UnkLightsIdPairIter_b54* out) {
    *out = UnkLightsIdPairIter_b54(self->count, self->data + self->count);
}
