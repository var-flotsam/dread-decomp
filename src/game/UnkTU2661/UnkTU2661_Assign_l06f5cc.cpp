// Unit 2661 (CCutscene::CTunableCutscene): copy-assigns an engine array of 0x30-byte polymorphic holders.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU2661.h"
#include "types.h"

// The 27 bytes after the first word, copied as one block.
struct __attribute__((packed)) UnkTunableData_l06f5cc {
    u8 bytes[27];
};

struct UnkTunableHolder_l06f5cc;
struct UnkTunableHolderVtable_l06f5cc {
    void (*dtor)(UnkTunableHolder_l06f5cc* self);
};

// A 0x30-byte element with a virtual destructor (vtable g_unk_7101cac988) and plain data after it.
struct UnkTunableHolder_l06f5cc {
    const UnkTunableHolderVtable_l06f5cc* vtable;
    u64 field_0x8;
    UnkTunableData_l06f5cc data;  // 0x10
    u8 pad_0x2b[5];

    UnkTunableHolder_l06f5cc(const UnkTunableHolder_l06f5cc& other)
        : vtable(reinterpret_cast<const UnkTunableHolderVtable_l06f5cc*>(g_unk_7101cac988 + 2)),
          field_0x8(other.field_0x8), data(other.data) {}
    UnkTunableHolder_l06f5cc(UnkTunableHolder_l06f5cc&& other)
        : vtable(reinterpret_cast<const UnkTunableHolderVtable_l06f5cc*>(g_unk_7101cac988 + 2)),
          field_0x8(other.field_0x8), data(other.data) {}
    ~UnkTunableHolder_l06f5cc() { vtable->dtor(this); }
};
static_assert(sizeof(UnkTunableHolder_l06f5cc) == 0x30, "holder size");
using Holder = UnkTunableHolder_l06f5cc;

base::global::CRntVector<Holder>* fn_710106f5cc(base::global::CRntVector<Holder>* self,
                                                const base::global::CRntVector<Holder>* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~Holder();
        self->count = 0;
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
    return self;
}
