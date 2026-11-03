// Unit 208: for each index in a list, samples a value from a source (slot 0x10), turns it into
// a vector (2 or 3 words) and stores it in the target's array at that index (fn_710016de34,
// fn_710016d7c8). Names and layouts are placeholders inferred from the code.

#include "types.h"

// Copied as words that may alias anything: the original reloads the target after the stores.
typedef u32 __attribute__((may_alias)) UnkWord_710016de34;

struct UnkVec3_710016de34 {
    u32 x, y, z;
    UnkVec3_710016de34& operator=(const UnkVec3_710016de34& other) {
        *reinterpret_cast<UnkWord_710016de34*>(&x) = other.x;
        *reinterpret_cast<UnkWord_710016de34*>(&y) = other.y;
        *reinterpret_cast<UnkWord_710016de34*>(&z) = other.z;
        return *this;
    }
};

struct UnkVec2_710016d7c8 {
    u32 x, y;
    UnkVec2_710016d7c8& operator=(const UnkVec2_710016d7c8& other) {
        *reinterpret_cast<UnkWord_710016de34*>(&x) = other.x;
        *reinterpret_cast<UnkWord_710016de34*>(&y) = other.y;
        return *this;
    }
};

void fn_710016dae4(UnkVec3_710016de34* out, f32 value);
void fn_7100152c1c(UnkVec2_710016d7c8* out, f32 value);

struct UnkTarget_710016de34 {
    u64 field_0x0;
    void* field_0x8;
    u8 pad_0x10[0x8];
    UnkVec2_710016d7c8* values2;
    UnkVec3_710016de34* values3;
};

struct UnkSource_710016de34 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual f32 vf_0x10(void* data, u32 index);
};

template <class V>
struct UnkSampler_710016de34 {
    u64 field_0x0;
    UnkTarget_710016de34* target;
    UnkSource_710016de34* source;
    f32 scale;
    u32 field_0x1c;
    V value;
};

template <class V, void (*Make)(V*, f32), V* UnkTarget_710016de34::*Values>
__attribute__((always_inline)) inline void SampleIndexed(UnkSampler_710016de34<V>* self, const u32* indices,
                                                         u32 count) {
    if (!count)
        return;
    do {
        u32 index = *indices++;
        f32 v = self->source->vf_0x10(self->target->field_0x8, index);
        Make(&self->value, v * self->scale);
        (self->target->*Values)[index] = self->value;
    } while (--count);
}

void fn_710016de34(UnkSampler_710016de34<UnkVec3_710016de34>* self, const u32* indices, u32 count) {
    SampleIndexed<UnkVec3_710016de34, fn_710016dae4, &UnkTarget_710016de34::values3>(self, indices, count);
}

void fn_710016d7c8(UnkSampler_710016de34<UnkVec2_710016d7c8>* self, const u32* indices, u32 count) {
    SampleIndexed<UnkVec2_710016d7c8, fn_7100152c1c, &UnkTarget_710016de34::values2>(self, indices, count);
}
