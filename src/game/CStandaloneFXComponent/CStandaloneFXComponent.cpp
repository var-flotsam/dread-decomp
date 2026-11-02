// Destructors of a component (vtable 0x7101a75008) holding an array (base::UnkVector's layout) of
// 0x40-byte records released by fn_710081348c; the same shape as fn_7100b19f44.
#include <new>

#include "base/UnkVector.h"
#include "game/UnkComponent.h"
#include "types.h"

extern void* g_unk_7101a75008[];

struct UnkStruct_710081348c;
void fn_710081348c(UnkStruct_710081348c* self);

struct UnkFxRecord {
    u8 data[0x40];
    __attribute__((always_inline)) ~UnkFxRecord() { fn_710081348c(reinterpret_cast<UnkStruct_710081348c*>(this)); }
};

struct UnkFxRecordArray {
    UnkFxRecord* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkFxRecordArray() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkFxRecord();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkFxComponentBase : UnkBaseComponent {
    __attribute__((always_inline)) ~UnkFxComponentBase() { fn_71010c4194(this); }
};

struct UnkFxComponent : UnkFxComponentBase {
    u8 unk_0x40[0x10];
    UnkFxRecordArray records;

    __attribute__((always_inline)) ~UnkFxComponent() { vtable = g_unk_7101a75008 + 2; }
};

void fn_71008129e0(UnkFxComponent* self) { self->~UnkFxComponent(); }
void fn_7100812a80(UnkFxComponent* self) {
    self->~UnkFxComponent();
    operator delete(self);
}
