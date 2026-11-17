// Unit 1629: the move assignment of a 0x80-byte graph node (its keyed collection's assign clone): takes the
// other node's id, links and names, then points every linked node's link back to the other node at this one.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkStrIdMember;
using base::global::CRntVector;
using base::global::CStrId;

struct UnkNode_lacfe74 {
    u32 field_0x0;
    u32 field_0x4;
    CStrId name;  // 0x8
    u8 field_0x10;
    void* field_0x18;
    CRntVector<UnkNode_lacfe74*> links;  // 0x20
    CRntVector<UnkStrIdMember> names;    // 0x48
    void* field_0x70;
    u8 field_0x78;
};
static_assert(__builtin_offsetof(UnkNode_lacfe74, field_0x78) == 0x78, "layout");

void* fn_7100acfe74(void* dst, const void* src) {
    UnkNode_lacfe74* self = static_cast<UnkNode_lacfe74*>(dst);
    UnkNode_lacfe74* other = const_cast<UnkNode_lacfe74*>(static_cast<const UnkNode_lacfe74*>(src));
    self->field_0x0 = other->field_0x0;
    self->field_0x4 = other->field_0x4;
    self->name.ReleaseAndTake(other->name);
    self->field_0x10 = other->field_0x10;
    self->field_0x18 = other->field_0x18;
    self->links = static_cast<CRntVector<UnkNode_lacfe74*>&&>(other->links);
    self->names = static_cast<CRntVector<UnkStrIdMember>&&>(other->names);
    self->field_0x70 = other->field_0x70;
    self->field_0x78 = other->field_0x78;
    // The linked nodes point at the other node: point them at this one.
    for (UnkNode_lacfe74* node : self->links) {
        for (u32 i = 0; i < node->links.count; ++i) {
            if (node->links.data[i] == other) {
                node->links.data[i] = self;
                break;
            }
        }
    }
    other->field_0x18 = nullptr;
    other->field_0x70 = nullptr;
    return self;
}
