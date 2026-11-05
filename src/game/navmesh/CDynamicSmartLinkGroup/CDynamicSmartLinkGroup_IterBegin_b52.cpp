// navmesh::CDynamicSmartLinkGroup's unit (tools/tu_map.py unit 351): the begin and end functions
// (slots 0x160 and 0x168 in src/reflection/navmesh_CDynamicSmartLinkGroup.cpp) of a reflected
// collection of 0x10-byte entries (a file-path id and a counted resource reference); copies of
// fn_710000f164 / fn_710000f244 (CBlendSpaceResource_IterBeginEnd_b49.cpp) with smaller entries. The
// iterator's constructor and assignment re-point its cached value through fn_7100266c44, kept out of
// line. Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

// A counted reference released by its destructor.
struct UnkSmartLinkIterRef_b52 {
    UnkCountedResource* ptr;

    ~UnkSmartLinkIterRef_b52() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

struct UnkSmartLinkIterEntry_b52 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> field_0x0;
    UnkSmartLinkIterRef_b52 field_0x8;
};
static_assert(sizeof(UnkSmartLinkIterEntry_b52) == 0x10, "UnkSmartLinkIterEntry_b52 layout");

struct UnkSmartLinkIterArray_b52 {
    UnkSmartLinkIterEntry_b52* data;
    u32 count;
};

// Re-points an iterator's cached value at (index, element); signature as
// src/base/animation/CBlendSpaceResource/UnkVariantHolderSet_b42.cpp defines it.
template <u32 Skip>
struct UnkVariantHolder_b42a1;
struct UnkStruct_0x10;
// fn_7100266c44 (signature as src/flag_id/fn_7100266c04.cpp declares it; its second argument is unused).
void UnkIterUpdate_b52(void* self) asm("_Z13fn_7100266c44P14UnkStruct_0x10PKS_");

struct UnkSmartLinkIter_b52 {
    u32 index;
    UnkSmartLinkIterEntry_b52* element;
    base::UnkIterValue<UnkSmartLinkIterEntry_b52, u32> current;

    UnkSmartLinkIter_b52(u32 index, UnkSmartLinkIterEntry_b52* element) : index(index), element(element) {
        UnkIterUpdate_b52(this);
    }
    UnkSmartLinkIter_b52& operator=(const UnkSmartLinkIter_b52& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            UnkIterUpdate_b52(this);
        }
        return *this;
    }
};

void fn_7100268dd8(UnkSmartLinkIterArray_b52* self, UnkSmartLinkIter_b52* out) {
    *out = UnkSmartLinkIter_b52(0, self->data);
}

void fn_7100268eb8(UnkSmartLinkIterArray_b52* self, UnkSmartLinkIter_b52* out) {
    *out = UnkSmartLinkIter_b52(self->count, self->data + self->count);
}
