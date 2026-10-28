// Destructors of a class (vtable 0x7101a9a628) with a string at 8 and an array (base::UnkVector's
// layout) of 0x38-byte records holding a string at 8: the records are destroyed and the array
// freed, then the string.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

extern void* g_unk_7101a9a628[];

struct UnkNamedRecord38 {
    u64 field_0x0;
    CRntString name;
};

struct UnkNamedRecordArray {
    UnkNamedRecord38* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkNamedRecordArray() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkNamedRecord38();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkNamedRecordOwner {
    void** vtable;
    CRntString name;
    u64 field_0x38;
    UnkNamedRecordArray records;

    __attribute__((always_inline)) ~UnkNamedRecordOwner() {
        *reinterpret_cast<void***>(this) = g_unk_7101a9a628 + 2;
    }
};

void fn_7100b19f44(UnkNamedRecordOwner* self) { self->~UnkNamedRecordOwner(); }
void fn_7100b19fe4(UnkNamedRecordOwner* self) {
    self->~UnkNamedRecordOwner();
    operator delete(self);
}
void fn_7100b1a128(UnkNamedRecordOwner* self) {
    self->~UnkNamedRecordOwner();
    operator delete(self);
}
