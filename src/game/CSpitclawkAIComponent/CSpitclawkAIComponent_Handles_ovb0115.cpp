// CSpitclawkAIComponent's unit (tools/tu_map.py unit 1067): after fn_71010ff2b8, every handle in the array at
// +0x146f8 whose object is still there has that object deleted (fn_7100232c68), then all the handles are
// released and the array emptied. Names and layouts are placeholders.
#include "game/UnkTU2702.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSpitclawkHandles_ovb0115 {
    UnkWeakHandle** data;
    u32 count;
};

struct UnkSpitclawkAI_ovb0115 {
    u8 pad_0x0[0x146f8];
    UnkSpitclawkHandles_ovb0115 handles;  // 0x146f8
};

// Lets each handle's object go (a temporary reference held around the call), then releases them all.
static inline void ClearHandles(UnkSpitclawkHandles_ovb0115* handles) {
    for (UnkWeakHandle** it = handles->data, **end = handles->data + handles->count; it != end; ++it) {
        UnkWeakHandle* handle = *it;
        if (!handle)
            continue;
        ++handle->ref_count;
        if (handle->target)
            fn_7100232c68(handle->target);
        if (--handle->ref_count == 0)
            handle->vf_0x10();
    }
    for (u64 i = 0; i < handles->count; ++i) {
        if (UnkWeakHandle* handle = handles->data[i]) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    handles->count = 0;
}

void fn_71006c8ce0(UnkSpitclawkAI_ovb0115* self) {
    fn_71010ff2b8(reinterpret_cast<char*>(self));
    ClearHandles(&self->handles);
}
