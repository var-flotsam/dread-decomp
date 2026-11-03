// From unit 211 of tools/tu_map.py, beside fn_7100193810 and fn_7100194508 (UnkUnit211Accumulate.cpp):
// the same two passes over a list of indices, multiplying the entry by the value read before the
// source's slot 0x10 is called with the index instead of adding it. Names and layouts are placeholders.
#include "types.h"

typedef f32 __attribute__((may_alias)) UnkUnit211ScaleFloat;

struct UnkUnit211ScaleVec3 {
    UnkUnit211ScaleFloat v[3];
};

struct UnkUnit211ScaleValues {
    u8 pad_0x0[0x18];
    UnkUnit211ScaleFloat* floats;  // 0x18
    UnkUnit211ScaleVec3* vectors;  // 0x20
};

struct UnkUnit211ScaleSource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(u32 index);
};

struct UnkUnit211ScalePass {
    void* field_0x0;
    UnkUnit211ScaleValues* values;  // 0x8
    UnkUnit211ScaleSource* source;  // 0x10
};

void fn_7100193974(UnkUnit211ScalePass* self, const u32* indices, u32 count) {
    for (; count != 0; --count) {
        u32 index = *indices++;
        f32 v = self->values->floats[index];
        self->source->vf_0x10(index);
        UnkUnit211ScaleFloat* e = &self->values->floats[index];
        *e = v * *e;
    }
}

void fn_71001946c0(UnkUnit211ScalePass* self, const u32* indices, u32 count) {
    for (; count != 0; --count) {
        u32 index = *indices++;
        const UnkUnit211ScaleFloat* v = self->values->vectors[index].v;
        f32 x = v[0];
        f32 y = v[1];
        f32 z = v[2];
        self->source->vf_0x10(index);
        UnkUnit211ScaleFloat* e = self->values->vectors[index].v;
        e[0] = x * e[0];
        e[1] = y * e[1];
        e[2] = z * e[2];
    }
}
