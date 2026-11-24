// Unit 107 (the kd-tree builder's boxes): the mean squared distance per axis from a point to the
// centres of a range of boxes, growing the box array on demand. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU107.h"
#include "game/UnkXformNode.h"
#include "types.h"

typedef u32 a_u32_b0236;

// A 2D box and a word, copied word by word through an aliasing type (not trivially copyable).
struct UnkBox_b0236 {
    u32 words[5];

    UnkBox_b0236(const UnkBox_b0236& o) {
        for (u32 i = 0; i < 5; ++i)
            reinterpret_cast<a_u32_b0236*>(words)[i] = reinterpret_cast<const a_u32_b0236*>(o.words)[i];
    }
    f32 Float(u32 i) const { return reinterpret_cast<const f32*>(words)[i]; }
};

// The boxes' array, with the layout of base::global::CRntVector.
struct UnkBoxVector_b0236 {
    UnkBox_b0236* data;  // 0x0
    u32 count;           // 0x8
    u32 capacity;        // 0xc
    u32 step;            // 0x10
    bool grows;          // 0x14
    u32 flags;           // 0x18
    base::UnkAllocator* allocator;  // 0x20

    void Reserve(u32 n) {
        if (capacity < n)
            Reallocate(n);
    }
    void Reallocate(u32 new_capacity) {
        UnkBox_b0236* p = static_cast<UnkBox_b0236*>(allocator->vf_0x40((u64)new_capacity * sizeof(UnkBox_b0236), 8, flags));
        if (data) {
            UnkBox_b0236* src = data;
            UnkBox_b0236* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src)
                new (dst) UnkBox_b0236(*src);
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }
};


// The box at `index`, growing the array to hold it when it may grow.
static inline UnkBox_b0236& BoxAt(UnkBoxVector_b0236* boxes, u32 index) {
    if (boxes->grows && boxes->count <= index) {
        if (boxes->capacity <= index && boxes->step != 0) {
            u32 grown = (index / boxes->step + 1) * boxes->step;
            if (boxes->capacity < grown)
                boxes->Reallocate(grown);
        }
        ResizeBoxVector(boxes, index + 1);
    }
    return boxes->data[index];
}

UnkVec3f ComputeBoxCentersMeanSquaredDistanceToPoint(const void* self, UnkBoxVector_b0236* boxes, u32 begin, u32 end, const UnkVec3f* point) {
    UnkVec3f sum(0.0f, 0.0f, 0.0f);
    u32 n = end - begin;
    for (u32 i = begin; i < end; ++i) {
        const UnkBox_b0236& box = BoxAt(boxes, i);
        f32 cx = (box.Float(0) + box.Float(2)) * 0.5f;
        f32 cy = (box.Float(1) + box.Float(3)) * 0.5f;
        f32 cz = 0.0f;
        f32 dx = cx - point->x;
        f32 dy = cy - point->y;
        f32 dz = cz - point->z;
        sum.x += dx * dx;
        sum.y += dy * dy;
        sum.z += dz * dz;
    }
    f32 count = (f32)n;
    sum.x /= count;
    sum.y /= count;
    sum.z /= count;
    return sum;
}
