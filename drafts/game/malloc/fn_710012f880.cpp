// Unit 110 (malloc). Fills a vertex array with the four corners of a quad taken from a pair of
// corner points, and an index array with its two triangles (winding chosen by a flag).
// Names and layouts are placeholders.
#include "types.h"

struct UnkQuadAllocator {
    virtual void vf_0x00();
    virtual void vf_0x08();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* Alloc(u64 size, u32 align, u32 tag);
    virtual void Free(void* ptr);
};

struct UnkQuadVertex {
    u32 x;
    u32 y;
    u32 z;

    UnkQuadVertex(u32 x_, u32 y_, u32 z_) : x(x_), y(y_), z(z_) {}
    UnkQuadVertex& operator=(const UnkQuadVertex& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkQuadCorners {
    u32 x0;
    u32 y0;
    u32 z0;
    u32 x1;
    u32 y1;
    u32 z1;
};

inline void CopyElements(u16* dst, const u16* src, const u32& count) {
    __builtin_memcpy(dst, src, static_cast<u64>(count) * sizeof(u16));
}

inline void CopyElements(UnkQuadVertex* dst, const UnkQuadVertex* src, const u32& count) {
    for (u32 i = 0; i < count; ++i) {
        *dst = *src;
        ++src;
        ++dst;
    }
}

template <typename T>
struct UnkQuadArray {
    T* data;
    u32 size;
    u32 capacity;
    u32 grow;
    u32 pad_0x14;
    u32 tag;
    u32 pad_0x1c;
    UnkQuadAllocator* allocator;

    void Clear() { size = 0; }

    __attribute__((always_inline)) void Reserve(u32 newCapacity) {
        if (capacity < newCapacity) {
            T* newData = static_cast<T*>(allocator->Alloc(static_cast<u64>(newCapacity) * sizeof(T), 8, tag));
            if (data != nullptr) {
                CopyElements(newData, data, size);
                allocator->Free(data);
            }
            data = newData;
            capacity = newCapacity;
        }
    }

    __attribute__((always_inline)) void PushBack(const T& value) {
        u32 newCapacity = capacity;
        if (capacity <= size) {
            if (grow != 0) {
                newCapacity = (size / grow + 1) * grow;
            }
        }
        if (static_cast<s32>(size) >= static_cast<s32>(newCapacity)) {
            for (;;) {
            }
        }
        Reserve(newCapacity);
        data[size] = value;
        ++size;
    }
};

void fn_710012f880(const UnkQuadCorners* corners, UnkQuadArray<UnkQuadVertex>* vertices,
                   UnkQuadArray<u16>* indices, bool flip) {
    const u32 x0 = corners->x0;
    const u32 y0 = corners->y0;
    const u32 z0 = corners->z0;
    const u32 x1 = corners->x1;
    const u32 y1 = corners->y1;
    const u32 z1 = corners->z1;

    vertices->Clear();
    vertices->Reserve(4);
    vertices->PushBack(UnkQuadVertex(x0, y0, z0));
    vertices->PushBack(UnkQuadVertex(x1, y0, z1));
    vertices->PushBack(UnkQuadVertex(x1, y1, z1));
    vertices->PushBack(UnkQuadVertex(x0, y1, z1));

    indices->Clear();
    indices->Reserve(12);
    if (flip) {
        indices->PushBack(0);
        indices->PushBack(1);
        indices->PushBack(2);
        indices->PushBack(0);
        indices->PushBack(2);
        indices->PushBack(3);
    } else {
        indices->PushBack(0);
        indices->PushBack(2);
        indices->PushBack(1);
        indices->PushBack(0);
        indices->PushBack(3);
        indices->PushBack(2);
    }
}
