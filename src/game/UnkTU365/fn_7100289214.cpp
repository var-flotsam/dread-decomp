// Unit 365 (navmesh::CNavMeshGenerator): assigns a 0x48-byte record from another: the list at +0 (out
// of line), two values and a flag, then the record's own list of 0x18-byte nodes is emptied and
// refilled with empty nodes, one for each entry of the source. Names and layouts are placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU108.h"
#include "types.h"

struct UnkNode_l289214 {
    u64 field_0x0;
    u32 field_0x8;
    UnkNode_l289214* next;  // 0x10
};

struct UnkRecord_l289214 {
    u8 pad_0x0[8];
    u32 entries;  // 0x8
    u8 pad_0xc[0x1c];
    u32 field_0x28;
    u8 field_0x2c;
    bool field_0x2d;
    u8 pad_0x2e[2];
    u8 pad_0x30[0x10];
    u32 field_0x40;
    u8 pad_0x44[0xc];
    base::UnkLinkedList<UnkNode_l289214> nodes;  // 0x50
};

static inline u32 Count_l289214(const UnkRecord_l289214* r) {
    bool unset = r->field_0x2c == 0;
    u32 n = r->entries;
    return n - (unset & (n != 0));
}

void fn_7100289214(UnkRecord_l289214* self, const UnkRecord_l289214* other) {
    self->field_0x28 = other->field_0x28;
    self->field_0x2c = other->field_0x2c;
    self->field_0x2d = true;
    fn_710011dac0(reinterpret_cast<UnkPartsRecord*>(self), reinterpret_cast<const UnkPartsRecord*>(other));
    self->field_0x40 = other->field_0x40;
    self->nodes.Clear();
    for (u32 i = 0; i < Count_l289214(other); ++i) {
        void* mem = self->nodes.allocator->vf_0x40(sizeof(UnkNode_l289214), 8, self->nodes.flags);
        if (mem)
            self->nodes.PushBack(new (mem) UnkNode_l289214{0, 0, nullptr});
    }
}
