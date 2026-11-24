// Unit 107 (2D shapes): the average center of a range of boxes in a lazily grown box array.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkXformNode.h"
#include "types.h"
#include "game/UnkTU107.h"

struct UnkBoxVector_b0236;

struct UnkBoxData_l10df80 {
    f32 min_x, min_y, max_x, max_y;
    u32 word;
};

// The boxes' array. Reading an element past the count grows the array first.
struct UnkBoxVector_b0236 {
    UnkBoxData_l10df80* data;       // 0x0
    u32 count;                      // 0x8
    u32 capacity;                   // 0xc
    u32 step;                       // 0x10
    bool grows;                     // 0x14
    u32 flags;                      // 0x18
    base::IAllocator* allocator;  // 0x20

    __attribute__((always_inline)) UnkBoxData_l10df80* At(u64 i) {
        if (grows && i >= count) {
            if (i >= capacity && step != 0) {
                u32 new_capacity = ((u32)i / step + 1) * step;
                if (capacity < new_capacity) {
                    UnkBoxData_l10df80* p = static_cast<UnkBoxData_l10df80*>(
                        allocator->vf_0x40((u64)new_capacity * sizeof(UnkBoxData_l10df80), 8, flags));
                    if (data) {
                        UnkBoxData_l10df80* src = data;
                        UnkBoxData_l10df80* dst = p;
                        for (u32 k = 0; k < count; ++k, ++src, ++dst) {
                            for (u32 w = 0; w < 5; ++w)
                                reinterpret_cast<a_u32*>(dst)[w] = reinterpret_cast<a_u32*>(src)[w];
                        }
                        allocator->vf_0x48(data);
                    }
                    data = p;
                    capacity = new_capacity;
                }
            }
            ResizeBoxVector(this, (u32)i + 1);
        }
        return &data[i];
    }
};

UnkVec3f fn_710010df80(void* unused, UnkBoxVector_b0236* boxes, u32 first, u32 last) {
    UnkVec3f sum(0.0f, 0.0f, 0.0f);
    for (u32 i = first; i < last; ++i) {
        UnkBoxData_l10df80* box = boxes->At(i);
        f32 center_x = (box->min_x + box->max_x) * 0.5f;
        f32 center_y = (box->min_y + box->max_y) * 0.5f;
        sum.x += center_x;
        sum.y += center_y;
        sum.z += 0.0f;
    }
    f32 n = (f32)(last - first);
    sum.x = sum.x / n;
    sum.y = sum.y / n;
    sum.z = sum.z / n;
    return sum;
}
