// CSpitclawkAIComponent's unit (tools/tu_map.py unit 1067): after the base update (fn_710067c888), drops
// from the handle array at +0x146f8 every handle that is empty or whose object is gone, closing the gap
// each time. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

char* fn_710067c888(char* p0, f32 f0);

struct UnkSpitclawkHandles_l6c8b60 {
    UnkWeakHandle** data;
    u32 count;

    // Drops one reference to `handle` (the slot keeps its value).
    static void Release(UnkWeakHandle* handle) {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

struct UnkSpitclawkAI_l6c8b60 {
    u8 pad_0x0[0x146f8];
    UnkSpitclawkHandles_l6c8b60 handles;  // 0x146f8
};

// Drops every handle that is empty or whose object is gone, closing the gap each time.
static inline void RemoveDead(UnkSpitclawkHandles_l6c8b60* handles) {
    if (!handles->count)
        return;
    UnkWeakHandle** it = handles->data;
    while (handles->data + handles->count != it) {
        if (*it && (*it)->target) {
            ++it;
            continue;
        }
        // Removes the handle at `it`, moving the later ones down; `it` then holds the next one.
        u32 index = it - handles->data;
        if (index < handles->count) {
            UnkWeakHandle** data = handles->data;
            UnkSpitclawkHandles_l6c8b60::Release(data[index]);
            for (u64 i = index; i < handles->count - 1; ++i) {
                UnkWeakHandle* next = handles->data[i + 1];
                handles->data[i] = nullptr;
                AssignRef(&handles->data[i], next);
                UnkSpitclawkHandles_l6c8b60::Release(handles->data[i + 1]);
            }
            handles->count = handles->count - 1;
        }
        it = handles->data + index;
    }
}

void fn_71006c8b60(UnkSpitclawkAI_l6c8b60* self, f32 dt) {
    fn_710067c888(reinterpret_cast<char*>(self), dt);
    RemoveDead(&self->handles);
}
