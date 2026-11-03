// From unit 211 of tools/tu_map.py: two passes over a list of indices that add a value read before
// calling the source's slot 0x10 with the index back onto the same entry once the call returns: one over floats (at +0x18 of the target), one over 3-float vectors
// (at +0x20). Names and layouts are placeholders.
#include "types.h"

typedef f32 __attribute__((may_alias)) UnkUnit211Float;

struct UnkUnit211Vec3 {
    UnkUnit211Float v[3];
};

struct UnkUnit211Values {
    u8 pad_0x0[0x18];
    UnkUnit211Float* floats;  // 0x18
    UnkUnit211Vec3* vectors;  // 0x20
};

struct UnkUnit211Source {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(u32 index);
};

struct UnkUnit211Pass {
    void* field_0x0;
    UnkUnit211Values* values;  // 0x8
    UnkUnit211Source* source;  // 0x10
};

void fn_7100193810(UnkUnit211Pass* self, const u32* indices, u32 count) {
    for (; count != 0; --count) {
        u32 index = *indices++;
        f32 v = self->values->floats[index];
        self->source->vf_0x10(index);
        self->values->floats[index] += v;
    }
}

void fn_7100194508(UnkUnit211Pass* self, const u32* indices, u32 count) {
    for (; count != 0; --count) {
        u32 index = *indices++;
        const UnkUnit211Float* v = self->values->vectors[index].v;
        f32 x = v[0];
        f32 y = v[1];
        f32 z = v[2];
        self->source->vf_0x10(index);
        UnkUnit211Float* e = self->values->vectors[index].v;
        e[0] += x;
        e[1] += y;
        e[2] += z;
    }
}
