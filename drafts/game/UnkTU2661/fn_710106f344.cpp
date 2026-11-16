// Unit 2661 (CCutscene::CTunableCutscene): copy-constructs an engine array of 0x30-byte polymorphic holders.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU2661.h"
#include "types.h"

// 35 bytes copied as one block.
struct __attribute__((packed)) UnkElemData2661 {
    u8 bytes[35];
};

// A 0x30-byte element with a virtual destructor (vtable g_unk_7101cac988) and plain data after it.
struct UnkTunableHolder_l06f344 {
    virtual ~UnkTunableHolder_l06f344();
    UnkElemData2661 data;  // 0x8

    UnkTunableHolder_l06f344(const UnkTunableHolder_l06f344& other) {
        *reinterpret_cast<char**>(this) = reinterpret_cast<char*>(g_unk_7101cac988) + 0x10;
        data = other.data;
    }
    UnkTunableHolder_l06f344(UnkTunableHolder_l06f344&& other)
        : UnkTunableHolder_l06f344(static_cast<const UnkTunableHolder_l06f344&>(other)) {}
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
    if (other->count != 0) {
        const Holder* end = other->data + other->count;
        for (const Holder* p = other->data; p != end; ++p) {
            u32 n = self->count;
            u32 old_capacity = self->capacity;
            u32 new_capacity = old_capacity;
            if (old_capacity <= n && self->step != 0)
                new_capacity = (n / self->step + 1) * self->step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            Holder* items;
            if (old_capacity < new_capacity) {
                items = static_cast<Holder*>(
                    self->allocator->vf_0x40((u64)new_capacity * sizeof(Holder), 8, self->flags));
                if (Holder* src = self->data) {
                    Holder* dst = items;
                    for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                        new (dst) Holder(static_cast<Holder&&>(*src));
                        src->~Holder();
                    }
                    self->allocator->vf_0x48(self->data);
                }
                n = self->count;
                self->data = items;
                self->capacity = new_capacity;
            } else {
                items = self->data;
            }
            new (&items[n]) Holder(*p);
            ++self->count;
        }
    }
}
