// From unit 2575 of tools/tu_map.py (0x7100ecf93c-0x7100ecfb5c, no reflected class): a holder of a
// polymorphic object that gives a 3D factor two ways (vtable slots 0x10 and 0x18). Names and
// layouts are placeholders.
// Holds fn_7100ecfa10 (scales a vector by one of the factors).
#include "types.h"

// The factor the object hands back in memory (a user-provided destructor makes it so).
struct UnkScaleFactor {
    f32 x;
    f32 y;
    f32 z;
    ~UnkScaleFactor() {}
};

struct UnkScaleSource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkScaleFactor vf_0x10(void* a, void* b);
    virtual UnkScaleFactor vf_0x18(void* a, void* b);
};

struct UnkScaleHolder {
    UnkScaleSource* source;
};

struct UnkScaleVec3 {
    f32 x;
    f32 y;
    f32 z;
};

// Multiplies `v` component by component with the source's factor (the second one when `which` is
// set; `a` and `b` are passed on to it). Nothing happens without a source.
void fn_7100ecfa10(UnkScaleHolder* self, void* a, void* b, UnkScaleVec3* v, s32 which) {
    if (UnkScaleSource* source = self->source) {
        UnkScaleFactor f = which ? source->vf_0x18(a, b) : source->vf_0x10(a, b);
        v->x = f.x * v->x;
        v->y = f.y * v->y;
        v->z = f.z * v->z;
    }
}
