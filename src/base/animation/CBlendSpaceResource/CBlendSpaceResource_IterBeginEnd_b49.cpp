// base::animation::CBlendSpaceResource's unit (tools/tu_map.py unit 14): the begin and end
// functions (slots 0x160 and 0x168 in src/reflection/base_animation_CBlendSpaceResource.cpp) of
// its reflected collection of 0x18-byte entries (a word, a file-path id and a counted resource
// reference, as in CBlendSpaceResource_MapSet_b49.cpp). The iterator's constructor and assignment
// re-point its cached value through fn_710000c3dc, kept out of line. Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

// A counted reference released by its destructor.
struct UnkBlendIterRef_b49a1 {
    UnkCountedResource* ptr;

    ~UnkBlendIterRef_b49a1() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

struct UnkBlendIterEntry_b49a1 {
    u64 field_0x0;
    base::UnkStrIdMemberOf<g_unk_7101d47200> field_0x8;
    UnkBlendIterRef_b49a1 field_0x10;
};
static_assert(sizeof(UnkBlendIterEntry_b49a1) == 0x18, "UnkBlendIterEntry_b49a1 layout");

struct UnkBlendIterArray_b49a1 {
    UnkBlendIterEntry_b49a1* data;
    u32 count;
};

// Re-points an iterator's cached value at (index, element); signature as
// src/base/animation/CBlendSpaceResource/UnkVariantHolderSet_b42.cpp defines it.
template <u32 Skip>
struct UnkVariantHolder_b42a1;
void fn_710000c3dc(UnkVariantHolder_b42a1<0x10>* self);

struct UnkBlendIter_b49a1 {
    u32 index;
    UnkBlendIterEntry_b49a1* element;
    base::UnkIterValue<UnkBlendIterEntry_b49a1, u32> current;

    UnkBlendIter_b49a1(u32 index, UnkBlendIterEntry_b49a1* element) : index(index), element(element) {
        fn_710000c3dc(reinterpret_cast<UnkVariantHolder_b42a1<0x10>*>(this));
    }
    UnkBlendIter_b49a1& operator=(const UnkBlendIter_b49a1& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            fn_710000c3dc(reinterpret_cast<UnkVariantHolder_b42a1<0x10>*>(this));
        }
        return *this;
    }
};

void fn_710000f164(UnkBlendIterArray_b49a1* self, UnkBlendIter_b49a1* out) {
    *out = UnkBlendIter_b49a1(0, self->data);
}

void fn_710000f244(UnkBlendIterArray_b49a1* self, UnkBlendIter_b49a1* out) {
    *out = UnkBlendIter_b49a1(self->count, self->data + self->count);
}
