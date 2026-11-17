// Destructor of a string id followed by an array of string ids (four copies).
#include "base/global/CStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/UnkTwoStepStrId.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkIdVector {
    UnkStrIdTwoStep* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkIdVector() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkStrIdTwoStep();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkNamedIds {
    UnkStrIdTwoStep name;
    UnkIdVector ids;

    __attribute__((always_inline)) ~UnkNamedIds() {}
};

void fn_71009ba4ac(UnkNamedIds* self) {
    self->~UnkNamedIds();
}

void fn_7100a72af4(UnkNamedIds* self) {
    self->~UnkNamedIds();
}

void fn_7100a73894(UnkNamedIds* self) {
    self->~UnkNamedIds();
}

void fn_7100a73ec8(UnkNamedIds* self) {
    self->~UnkNamedIds();
}
