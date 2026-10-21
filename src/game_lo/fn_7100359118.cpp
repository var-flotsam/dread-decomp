// Constructor: stores the vtable, default-constructs eight arrays (allocator flags 0xf), an
// allocator handle and a ninth array (flags 0xe), sizes that array to 11 zeroed entries, and
// clears the remaining fields.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101aa9b20[];  // vtable

struct UnkAllocHandle_7100359118 {
    base::UnkAllocator* allocator = fn_7100083990();
    u32 flags = 0xe;
    u32 field_0xc = 0;
};

struct UnkObject_7100359118 {
    char* vtable;
    base::UnkVector<u64> field_0x8;
    base::UnkVector<u64> field_0x30;
    base::UnkVector<u64> field_0x58;
    base::UnkVector<u64> field_0x80;
    base::UnkVector<u64> field_0xa8;
    base::UnkVector<u64> field_0xd0;
    base::UnkVector<u64> field_0xf8;
    base::UnkVector<u64> field_0x120;
    UnkAllocHandle_7100359118 field_0x148;
    base::UnkVector<u64> field_0x158;
    u64 field_0x180;
    u64 field_0x188;
    bool field_0x190;
};

void fn_7100359118(UnkObject_7100359118* self) {
    self->vtable = g_unk_7101aa9b20 + 0x10;
    new (&self->field_0x8) base::UnkVector<u64>(0xf);
    new (&self->field_0x30) base::UnkVector<u64>(0xf);
    new (&self->field_0x58) base::UnkVector<u64>(0xf);
    new (&self->field_0x80) base::UnkVector<u64>(0xf);
    new (&self->field_0xa8) base::UnkVector<u64>(0xf);
    new (&self->field_0xd0) base::UnkVector<u64>(0xf);
    new (&self->field_0xf8) base::UnkVector<u64>(0xf);
    new (&self->field_0x120) base::UnkVector<u64>(0xf);
    new (&self->field_0x148) UnkAllocHandle_7100359118;
    new (&self->field_0x158) base::UnkVector<u64>(0xe);
    self->field_0x180 = 0;
    self->field_0x188 = 0;

    // Resize to 11 zeroed entries.
    base::UnkVector<u64>& v = self->field_0x158;
    u32 n = v.count;
    if (n < 11) {
        if (v.capacity < 11) {
            u64* p = static_cast<u64*>(v.allocator->vf_0x40(11 * sizeof(u64), 8, v.flags));
            if (v.data) {
                __builtin_memcpy(p, v.data, (u64)v.count * sizeof(u64));
                v.allocator->vf_0x48(v.data);
            }
            v.capacity = 11;
            n = v.count;
            v.data = p;
        }
        u64* p = v.data;
        __builtin_memset(&p[n], 0, (u64)(11 - n) * sizeof(u64));
        v.count = 11;
    } else if (n != 11) {
        v.count = 11;
    }
    self->field_0x190 = false;
}
