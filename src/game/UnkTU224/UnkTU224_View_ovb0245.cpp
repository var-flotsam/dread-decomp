// Unit 224 (tools/tu_map.py, no class hint): the view's matrix, rebuilt on first use from its position and scale
// (fn_710019d920, which also sets the aspect ratio of the size it is given), and the growth of an array of two-word
// elements to hold an index (fn_710019cf9c). Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "game/UnkTU178.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkView_ovb0245 {
    u8 pad_0x0[0x30];
    UnkVec3f position;  // 0x30
    UnkVec3f scale;     // 0x3c
    u8 pad_0x48[0x11c];
    UnkMatrix44 matrix;  // 0x164
    u8 pad_0x1a4[8];
    u8 matrix_valid;  // 0x1ac
};
static_assert(__builtin_offsetof(UnkView_ovb0245, matrix_valid) == 0x1ac, "layout");

struct UnkSize_ovb0245 {
    u8 pad_0x0[8];
    u32 width;   // 0x8
    u32 height;  // 0xc
};

UnkMatrix44 fn_71000791a8(const UnkVec3f&, const UnkVec3f&, const UnkVec3f&);

UnkMatrix44* fn_710019d920(UnkView_ovb0245* self, UnkSize_ovb0245* size) {
    if (!self->matrix_valid) {
        self->matrix_valid = 1;
        UnkMatrix44 m = fn_71000791a8(self->position, UnkVec3f(0.0f, 0.0f, 0.0f), self->scale);
        UnkCopyMatrix(&self->matrix, m);
        fn_710015bf40(&self->matrix, (f32)size->width / (f32)size->height);
    }
    return &self->matrix;
}


struct UnkPair_ovb0245 {
    typedef u32 __attribute__((may_alias)) U32A;
    U32A w[2];
    UnkPair_ovb0245() {
        w[0] = 0;
        w[1] = 0;
    }
    UnkPair_ovb0245(const UnkPair_ovb0245& o) {
        w[0] = o.w[0];
        w[1] = o.w[1];
    }
};

// As src/game/UnkFrameSource.cpp declares it (UnkFrameVector derives from UnkVector, UnkFramePair is the element above).
template <class T>
struct UnkFrameVector;
struct UnkFramePair;

UnkFramePair* fn_710019cf9c(UnkFrameVector<UnkFramePair>* v, u32 index) {
    auto* self = reinterpret_cast<base::UnkVector<UnkPair_ovb0245>*>(v);
    return reinterpret_cast<UnkFramePair*>(&self->GrowAt(index));
}
