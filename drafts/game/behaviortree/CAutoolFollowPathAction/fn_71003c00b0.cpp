// CAutoolFollowPathAction unit: copy constructor of a growable array (base::global::CRntVector) of
// 0x20-byte polymorphic path points (vtable g_unk_7101cac8f0, five words, destroyed through slot 1).
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

extern char g_unk_7101cac8f0[];

struct UnkFollowPathPoint;

struct UnkFollowPathPointVtable {
    void* slot_0x0;
    void (*destroy)(UnkFollowPathPoint* self);
};

struct UnkFollowPathPoint {
    UnkFollowPathPointVtable* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u8 pad_0x1c[0x4];

    UnkFollowPathPoint(const UnkFollowPathPoint& other)
        : vtable(reinterpret_cast<UnkFollowPathPointVtable*>(g_unk_7101cac8f0 + 0x10)),
          field_0x8(other.field_0x8), field_0xc(other.field_0xc), field_0x10(other.field_0x10),
          field_0x14(other.field_0x14), field_0x18(other.field_0x18) {}
    ~UnkFollowPathPoint() { vtable->destroy(this); }
};
static_assert(sizeof(UnkFollowPathPoint) == 0x20, "layout");

using UnkFollowPath = base::global::CRntVector<UnkFollowPathPoint>;

// CRntVector's copy constructor, written out for this element type.
void fn_71003c00b0(UnkSub* self_, const UnkSub* other_) {
    UnkFollowPath* self = reinterpret_cast<UnkFollowPath*>(self_);
    const UnkFollowPath* other = reinterpret_cast<const UnkFollowPath*>(other_);
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    if (other->count != 0) {
        const UnkFollowPathPoint* end = other->data + other->count;
        const UnkFollowPathPoint* p = other->data;
        do {
            u32 n = self->count;
            u32 new_capacity = self->capacity;
            if (new_capacity <= n && self->step != 0)
                new_capacity = (n / self->step + 1) * self->step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            self->Reserve(new_capacity);
            new (&self->data[self->count]) UnkFollowPathPoint(*p);
            ++self->count;
        } while (++p != end);
    }
}
