// nn::atk unit 2772: carves a caller-supplied memory block into a header area, two pools and an element area on
// first use and records the creation parameter. Names and layouts are placeholders.
#include "types.h"
#include "game/UnkTU2772.h"

struct UnkAtkPool_l22c7a0 {
    void* field_0x0;
    void* field_0x8;
    void* field_0x10;
};

struct UnkAtkPools_l22c7a0 {
    u8 pad_0x0[0x28];
    bool ready;  // 0x28
    u8 pad_0x29[3];
    u32 handle;  // 0x2c
    UnkAtkPool_l22c7a0* first;   // 0x30
    UnkAtkPool_l22c7a0* second;  // 0x38
    u64 element_size;  // 0x40
    u64 param;  // 0x48
};

u32 fn_710122acc0(UnkAtkPools_l22c7a0* self, u8* base, u64 size, u32 a, u32 b);
void fn_710122ac08(UnkAtkPool_l22c7a0* pool, u8* memory, u64 size, u64 element_size);

void fn_710122c7a0(UnkAtkPools_l22c7a0* self, u8* base, u64 size, s32 count, u64* param) {
    if (self->ready)
        return;
    u64 element_size = (fn_710120f124(param) + 3) & ~3UL;
    self->element_size = element_size;
    u64 header = element_size ? sizeof(UnkAtkPool_l22c7a0) : 0;
    u64 pool_bytes = element_size ? (s64)count * 3 * 8 : 0;
    u64 elements_bytes = element_size * count;
    size = size - elements_bytes - pool_bytes - header * 2;
    self->handle = fn_710122acc0(self, base, size, 0x400, 0x40);
    if (element_size) {
        UnkAtkPool_l22c7a0* first = reinterpret_cast<UnkAtkPool_l22c7a0*>(base + size);
        self->first = first;
        first->field_0x10 = nullptr;
        u8* pools = reinterpret_cast<u8*>(first) + header;
        first->field_0x8 = nullptr;
        first->field_0x0 = nullptr;
        fn_710122ac08(self->first, pools, pool_bytes, 0x18);
        UnkAtkPool_l22c7a0* second = reinterpret_cast<UnkAtkPool_l22c7a0*>(pools + pool_bytes);
        self->second = second;
        second->field_0x0 = nullptr;
        second->field_0x10 = nullptr;
        u8* elements = reinterpret_cast<u8*>(second) + header;
        second->field_0x8 = nullptr;
        UnkAtkPool_l22c7a0* pool = self->second;
        u64 extra = fn_710120f124(param);
        fn_710122ac08(pool, elements, elements_bytes, extra > 8 ? extra : 8);
    }
    self->ready = true;
    self->param = *param;
}
