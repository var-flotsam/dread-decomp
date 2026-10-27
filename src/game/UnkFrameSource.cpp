// From unit 250 of tools/tu_map.py (0x71001bcbec-0x71001bd6ec, initialiser fn_71001bd660, no class
// hint): the per-frame source that src/game/UnkFrameMetrics.cpp reads (the object at +0x18 of its
// owner): a frame count, a 16-bit count and per-frame vectors with bounds checks. Names and
// layouts are placeholders.
// Holds fn_71001bcdf0 (takes a resource and reads its per-frame data), fn_71001bd628 and
// fn_71001bd5a8.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

// base::global::CRntVector (the engine's growable array, base/UnkVector.h's layout) with its
// bounds-checked element access and its resize.
template <class T>
struct UnkFrameVector : base::UnkVector<T> {
    const T& operator[](u32 i) const {
        if (!(i < this->count))
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return this->data[i];
    }

    // Grows to `n` value-initialised elements or drops the ones past `n`. Trivially copyable
    // elements are moved with memcpy and cleared with memset, others one by one.
    void Resize(u32 n) {
        if (this->count < n) {
            if constexpr (__is_trivially_copyable(T)) {
                T* p;
                if (this->capacity < n) {
                    p = static_cast<T*>(this->allocator->vf_0x40((u64)n * sizeof(T), 8, this->flags));
                    if (this->data) {
                        __builtin_memcpy(p, this->data, (u64)this->count * sizeof(T));
                        this->allocator->vf_0x48(this->data);
                    }
                    this->data = p;
                    this->capacity = n;
                } else {
                    p = this->data;
                }
                __builtin_memset(&p[this->count], 0, (u64)(n - this->count) * sizeof(T));
            } else {
                if (this->capacity < n) {
                    T* p = static_cast<T*>(this->allocator->vf_0x40((u64)n * sizeof(T), 8, this->flags));
                    if (this->data) {
                        T* src = this->data;
                        T* dst = p;
                        for (u32 i = 0; i < this->count; ++i, ++dst, ++src)
                            new (dst) T(*src);
                        this->allocator->vf_0x48(this->data);
                    }
                    this->data = p;
                    this->capacity = n;
                }
                for (u32 i = this->count; i < n; ++i)
                    new (&this->data[i]) T();
            }
            this->count = n;
        } else if (n < this->count) {
            this->count = n;
        }
    }
};

// A per-frame pair of floats. The fields are union members (no type-based alias info): the
// original reloads the array's count and data pointer after storing one.
struct UnkFramePair {
    union {
        f32 x;
    };
    union {
        f32 y;
    };

    UnkFramePair() : x(0.0f), y(0.0f) {}
    UnkFramePair(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkFramePair(const UnkFramePair& other) {
        x = other.x;
        y = other.y;
    }
};

// The resource the source reads (base/UnkCountedResource.h): of kind 4, with the frame data at
// +0x70.
struct UnkFrameResource : UnkCountedResource {
    u8 pad_0x58[0x18];
    void* field_0x70;
    u32 field_0x78;
};

struct UnkFrameSource {
    u8 pad_0x0[0x78];
    UnkFrameResource* field_0x78;
    void* field_0x80;  // the resource's frame data
    u64 field_0x88;
    u32 field_0x90;
    u32 field_0x94;
    u32 field_0x98;  // the number of frames
    u16 field_0x9c;
    s16 field_0x9e;
    u16 field_0xa0;
    u16 field_0xa2;
    u8 pad_0xa4[0x4];
    UnkFrameVector<UnkFramePair> field_0xa8;
    UnkFrameVector<UnkFramePair> field_0xd0;
    UnkFrameVector<UnkFramePair> field_0xf8;
    UnkFrameVector<f32> field_0x120;
};
static_assert(__builtin_offsetof(UnkFrameSource, field_0x120) == 0x120, "UnkFrameSource placeholder layout");

// The frame data's counts and one frame's values.
u32 fn_71001ab3bc(void* data);
u32 fn_71001ab35c(void* data);
u32 fn_71001ab37c(void* data);
u16 fn_71001ab39c(void* data);
s32 fn_71001ab44c(void* data);
void fn_71001ab3dc(void* data, u32 frame, u16* a, u16* b, u16* c, u16* d, s16* e, s16* f, u16* g);

// The arrays' element accessors (growing the array past its end).
UnkFramePair* fn_710019cf9c(UnkFrameVector<UnkFramePair>* v, u32 i);
f32* fn_71000d4a50(UnkFrameVector<f32>* v, u32 i);

// Takes `resource` (counted) and, when it is of kind 4, reads its frame data: the counts, and per
// frame two pairs of 16-bit values, a pair scaled by the counts at +0x90 and +0x94, a signed pair
// and one more value; the 16-bit count at +0x9c ends up the tallest extent.
// NON_MATCHING(minor): register allocation only: where the frame's top is the new highest, its
// height is loaded into w8 and copied to w9 (the original loads w9 and copies it to w8).
void fn_71001bcdf0(UnkFrameSource* self, UnkFrameResource* resource) {
    if (self->field_0x80) {
        self->field_0x98 = 0;
        self->field_0xd0.count = 0;
        self->field_0xa8.count = 0;
        self->field_0xf8.count = 0;
        self->field_0x120.count = 0;
    }
    if (self->field_0x78 != resource) {
        if (resource)
            ++resource->ref_count;
        UnkFrameResource* old = self->field_0x78;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        self->field_0x78 = resource;
    }
    if (!resource || resource->field_0x78 != 4)
        return;
    self->field_0x80 = resource->field_0x70;
    self->field_0x98 = fn_71001ab3bc(self->field_0x80);
    self->field_0x88 = *reinterpret_cast<u64*>(static_cast<u8*>(self->field_0x80) + 0x28);
    self->field_0x90 = fn_71001ab35c(self->field_0x80);
    self->field_0x94 = fn_71001ab37c(self->field_0x80);
    self->field_0xa0 = fn_71001ab39c(self->field_0x80);
    self->field_0xa2 = fn_71001ab39c(self->field_0x80);
    self->field_0xa8.Resize(self->field_0x98);
    self->field_0xd0.Resize(self->field_0x98);
    self->field_0xf8.Resize(self->field_0x98);
    self->field_0x120.Resize(self->field_0x98);
    self->field_0x9c = 0;
    self->field_0x9e = 0;
    for (u32 i = 0; i < self->field_0x98; ++i) {
        u16 a = 0;
        u16 b = 0;
        u16 c = 0;
        u16 d = 0;
        s16 e = 0;
        s16 f = 0;
        u16 g = 0;
        fn_71001ab3dc(self->field_0x80, i, &a, &b, &c, &d, &e, &f, &g);
        if (i == 0) {
            self->field_0x9e = f;
            self->field_0x9c = d;
        } else {
            s16 extent;
            if (f > self->field_0x9e) {
                self->field_0x9e = f;
                extent = d;
            } else {
                extent = self->field_0x9e - f + d;
            }
            if (self->field_0x9c < extent)
                self->field_0x9c = extent;
        }
        *fn_710019cf9c(&self->field_0xa8, i) = UnkFramePair(c, d);
        *fn_710019cf9c(&self->field_0xd0, i) =
            UnkFramePair((f32)a / (f32)self->field_0x90, (f32)b / (f32)self->field_0x94);
        *fn_710019cf9c(&self->field_0xf8, i) = UnkFramePair(e, f);
        *fn_71000d4a50(&self->field_0x120, i) = g;
    }
    if (fn_71001ab44c(self->field_0x80) > 0)
        self->field_0x9c = fn_71001ab44c(self->field_0x80);
}

// The 16-bit count at +0x9c.
f32 fn_71001bd628(UnkFrameSource* self) {
    return self->field_0x9c;
}

// The vector at +0xa8 for `frame`; (0, the 16-bit count) past the last frame.
void fn_71001bd5a8(UnkFrameSource* self, u32 frame, CVector2D* out) {
    out->x = 0.0f;
    out->y = self->field_0x9c;
    if (frame < self->field_0x98) {
        const UnkFramePair& v = self->field_0xa8[frame];
        out->x = v.x;
        out->y = v.y;
    }
}
