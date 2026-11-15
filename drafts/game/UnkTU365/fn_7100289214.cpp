// Unit 365 (navmesh::CNavMeshGenerator): assigns a 0x48-byte record from another: the list at +0 (out
// of line), two values and a flag, then the record's own list of 0x18-byte nodes is emptied and
// refilled with empty nodes, one for each entry of the source. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU108.h"
#include "types.h"

struct UnkNode_l289214 {
    u64 field_0x0;
    u32 field_0x8;
    // A union member: the original reloads the list count after storing it.
    union {
        UnkNode_l289214* next;  // 0x10
    };
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
    u32 count;  // 0x50
    u8 pad_0x54[4];
    UnkNode_l289214* head;  // 0x58
    UnkNode_l289214* tail;  // 0x60
    base::UnkAllocator* allocator;  // 0x68
    u32 flags;  // 0x70
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
    for (UnkNode_l289214* n = self->head; n;) {
        UnkNode_l289214* next = n->next;
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    for (u32 i = 0; i < Count_l289214(other); ++i) {
        UnkNode_l289214* node = static_cast<UnkNode_l289214*>(self->allocator->vf_0x40(sizeof(UnkNode_l289214), 8, self->flags));
        if (node) {
            u32 count;
            node->field_0x0 = 0;
            node->field_0x8 = 0;
            node->next = nullptr;
            if (self->count == 0) {
                self->head = node;
                self->tail = node;
                count = 1;
            } else {
                self->tail->next = node;
                node->next = nullptr;
                count = self->count + 1;
                self->tail = node;
            }
            self->count = count;
        }
    }
}
