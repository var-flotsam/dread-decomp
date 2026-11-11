#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::global::CStrId;

struct UnkIdTable_b0261 {
    CStrId id;                 // 0x0
    UnkAllocator* allocator;   // 0x8
    u32 field_0x10;
    u32 field_0x14;
    u64* data;                 // 0x18
    u32 count;                 // 0x20
    u32 capacity;              // 0x24
    u32 step;                  // 0x28
    bool field_0x2c;
    u32 flags;                 // 0x30
    UnkAllocator* allocator2;  // 0x38
    u64 field_0x40;
    u64 field_0x48;
};

void fn_7100f89e44(UnkIdTable_b0261* self) {
    self->id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id.data = &g_unk_7101d47108->empty;
    self->allocator = fn_7100083990();
    self->field_0x10 = 0xe;
    self->field_0x14 = 0;
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = 10;
    self->field_0x2c = true;
    self->flags = 0xe;
    self->allocator2 = fn_7100083990();
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    u32 n = self->count;
    u64* p;
    if (n <= 10) {
        if (self->capacity < 11) {
            p = static_cast<u64*>(self->allocator2->vf_0x40(11 * sizeof(u64), 8, self->flags));
            if (self->data) {
                __builtin_memcpy(p, self->data, (u64)self->count * sizeof(u64));
                self->allocator2->vf_0x48(self->data);
            }
            self->capacity = 11;
            n = self->count;
            self->data = p;
        } else {
            p = self->data;
        }
        __builtin_memset(&p[n], 0, (u64)(11 - n) * sizeof(u64));
    } else if (n == 11) {
        return;
    }
    self->count = 11;
}
