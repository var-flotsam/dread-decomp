// animtree::CBoneFilterNode's unit (tools/tu_map.py): hands every bone name of the node's definition
// (+0x10) to the filter at +0x28, the names in the list at +0x10 (count +0x18) through fn_710014a92c and
// those in the list at +0x38 (count +0x40) through fn_710014a934, each name passed as the string text a
// copy of its id refers to (+0x30 of the string data). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkBoneFilter_ovb0126;
void fn_710014a92c(UnkBoneFilter_ovb0126* filter, const char* name);
void fn_710014a934(UnkBoneFilter_ovb0126* filter, const char* name);

struct UnkBoneFilterDef_ovb0126 {
    u8 pad_0x0[0x10];
    CStrId* included;  // 0x10
    u32 included_count;
    u8 pad_0x1c[0x1c];
    CStrId* excluded;  // 0x38
    u32 excluded_count;
};

struct UnkBoneFilterNode_ovb0126 {
    u8 pad_0x0[0x10];
    UnkBoneFilterDef_ovb0126* def;
    u8 pad_0x18[0x10];
    UnkBoneFilter_ovb0126* filter;  // 0x28
};

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A copy of a bone's name id; its release has no null test (the id was read through).
struct UnkBoneName_ovb0126 {
    UnkStringData* data;

    explicit UnkBoneName_ovb0126(const CStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkBoneName_ovb0126() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
        }
    }
    const char* Text() const { return *reinterpret_cast<const char* const*>(reinterpret_cast<const u8*>(data) + 0x30); }
};

void fn_71002e220c(UnkBoneFilterNode_ovb0126* self) {
    for (CStrId *it = self->def->included, *end = it + self->def->included_count; it != end; ++it) {
        UnkBoneName_ovb0126 name(*it);
        fn_710014a92c(self->filter, name.Text());
    }
    for (CStrId *it = self->def->excluded, *end = it + self->def->excluded_count; it != end; ++it) {
        UnkBoneName_ovb0126 name(*it);
        fn_710014a934(self->filter, name.Text());
    }
}
