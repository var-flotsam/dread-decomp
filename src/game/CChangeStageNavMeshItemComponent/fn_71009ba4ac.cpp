// Destructor of a string id followed by an array of string ids (four copies).
#include "base/global/CStrId.h"
#include "base/UnkVector.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id (CStrId's layout) whose destructor releases in two steps: decrement, then test
// the reloaded count. Deriving from CStrId would run its destructor as well.
struct UnkId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkIdVector {
    UnkId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkIdVector() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkId();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkNamedIds {
    UnkId name;
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
