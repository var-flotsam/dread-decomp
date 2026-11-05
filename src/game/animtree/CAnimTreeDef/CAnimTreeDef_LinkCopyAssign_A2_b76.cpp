// animtree::CAnimTreeDef::TAnimTreeLinkDef's copy assignment (fn_0x48 of its reflected class,
// src/reflection/animtree_CAnimTreeDef.cpp): a record of two string ids, each assigned with CStrId's copy
// assignment. The move assignment is fn_71002db2a0.cpp. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkLinkDef_A2_b76 {
    CStrId field_0x0;
    CStrId field_0x8;
};

void* fn_71002db1b4(void* dst, const void* src) {
    UnkLinkDef_A2_b76* self = static_cast<UnkLinkDef_A2_b76*>(dst);
    const UnkLinkDef_A2_b76* other = static_cast<const UnkLinkDef_A2_b76*>(src);
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        self->field_0x8 = other->field_0x8;
    }
    return self;
}
