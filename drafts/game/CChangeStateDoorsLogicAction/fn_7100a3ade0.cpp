// CChangeStateDoorsLogicAction unit: move-assigns a two-mode holder (value stored inline, or
// stored behind pointers) from another one. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "types.h"

struct UnkDoorPayload {
    u64 field_0x0;
    UnkErasedVector vec;
    u32 count;
    u32 pad_0x34;
    base::UnkRefHandle handle;
    u32 tail;
};

struct UnkDoorHolder {
    union {
        u32 id;
        u32* id_ptr;
    };
    union {
        UnkDoorPayload payload;
        UnkDoorPayload* payload_ptr;
    };
    u32 mode;
};
static_assert(sizeof(UnkDoorPayload) == 0x48, "UnkDoorPayload layout");
static_assert(__builtin_offsetof(UnkDoorHolder, mode) == 0x50, "UnkDoorHolder layout");

UnkDoorHolder* fn_7100a3ade0(UnkDoorHolder* self, UnkDoorHolder* other) {
    UnkDoorPayload* dst;
    UnkDoorPayload* src;
    u32 id;
    if (other->mode == 1) {
        id = other->id;
        u32* dst_id = self->mode == 2 ? self->id_ptr : &self->id;
        *dst_id = id;
        src = &other->payload;
        dst = self->mode == 2 ? self->payload_ptr : &self->payload;
    } else if (other->mode == 2) {
        id = *other->id_ptr;
        u32* dst_id = self->mode == 2 ? self->id_ptr : &self->id;
        *dst_id = id;
        src = other->payload_ptr;
        dst = self->mode == 2 ? self->payload_ptr : &self->payload;
    } else {
        return self;
    }
    base::UnkRefHandle* src_handle = &src->handle;
    fn_710031d208(&dst->handle, src_handle);
    dst->count = src->count;
    UnkErasedVector* src_vec = &src->vec;
    fn_710023b29c(&dst->vec, src_vec);
    src->count = 0;
    fn_7100ad3588(src_vec);
    if (base::UnkRefCounted* p = src_handle->ptr) {
        if (--p->ref_count == 0)
            delete p;
        src_handle->ptr = nullptr;
    }
    dst->tail = *(u32*)((u8*)src + 0x40);
    return self;
}
