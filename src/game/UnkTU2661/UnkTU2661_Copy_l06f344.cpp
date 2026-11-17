// Unit 2661 (CCutscene::CTunableCutscene): copy-constructs an engine array of 0x30-byte polymorphic holders.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU2661.h"
#include "types.h"

// The 27 bytes after the first word, copied as one block.
struct __attribute__((packed)) UnkElemData2661 {
    u8 bytes[27];
};

struct UnkTunableHolder_l06f344;
struct UnkTunableHolderVtable_l06f344 {
    void (*dtor)(UnkTunableHolder_l06f344* self);
};

// A 0x30-byte element with a virtual destructor (vtable g_unk_7101cac988) and plain data after it.
struct UnkTunableHolder_l06f344 {
    const UnkTunableHolderVtable_l06f344* vtable;
    u64 field_0x8;
    UnkElemData2661 data;  // 0x10
    u8 pad_0x2b[5];

    UnkTunableHolder_l06f344(const UnkTunableHolder_l06f344& other)
        : vtable(reinterpret_cast<const UnkTunableHolderVtable_l06f344*>(g_unk_7101cac988 + 2)),
          field_0x8(other.field_0x8), data(other.data) {}
    UnkTunableHolder_l06f344(UnkTunableHolder_l06f344&& other)
        : vtable(reinterpret_cast<const UnkTunableHolderVtable_l06f344*>(g_unk_7101cac988 + 2)),
          field_0x8(other.field_0x8), data(other.data) {}
    ~UnkTunableHolder_l06f344() { vtable->dtor(this); }
};
static_assert(sizeof(UnkTunableHolder_l06f344) == 0x30, "holder size");
using Holder = UnkTunableHolder_l06f344;

void fn_710106f344(base::global::CRntVector<UnkTunableHolder_l06f344>* self,
                   const base::global::CRntVector<UnkTunableHolder_l06f344>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const Holder *p = other->data, *end = other->data + other->count; p != end; ++p) {
        // CRntVector::PushBack, inlined: grow to the next multiple of the step when full.
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        self->Reserve(new_capacity);
        new (&self->data[self->count]) Holder(*p);
        ++self->count;
    }
}
