// The world matrix of one bone of a skinned model, if it exists: the owner's transform matrix
// (UnkXform at +0x80, include/game/UnkXformNode.h) times the bone's matrix (fn_71000127c8). The
// bones (0x98-byte entries keyed by u64 ids, in the skeleton reached through +0x78 and +0x80) are
// updated first (fn_710019b9c8). By index (fn_71001c1b70) or by id (fn_71001c1694).
#include "base/UnkCloneCalls.h"
#include "game/UnkXformNode.h"

struct UnkBone_0x98 {
    u8 pad_0x0[0x18];
    UnkMatrix44 matrix;
    u8 pad_0x58[0x40];
};

struct UnkBoneArrayTag;

struct UnkBoneArray {
    u8 pad_0x0[0x8];
    union {
        UnkBone_0x98* bones;
        base::UnkKeyedArray<UnkBoneArrayTag, u64, UnkBone_0x98> by_id;
    };
};

struct UnkBoneArrayHolder {
    u8 pad_0x0[0x80];
    UnkBoneArray* bones;
    UnkMatrix44 matrix;
};

// Updated with a size (fn_710021693c).
struct UnkSized {
    u8 pad_0x0[0x8];
    f32 size;
};

struct UnkBoneOwner {
    u8 pad_0x0[0x20];
    UnkSized sized;
    u8 pad_0x2c[0x4c];
    UnkBoneArrayHolder* holder;
    UnkXform xform;
};

void fn_710019b9c8(UnkBoneArray* bones);
UnkMatrix44 fn_71000127c8(const UnkMatrix44& a, const UnkMatrix44& b);  // a * b

bool fn_71001c1b70(UnkBoneOwner* self, u32 index, UnkMatrix44* out) {
    UnkBoneArray* bones = self->holder->bones;
    fn_710019b9c8(bones);
    UnkBone_0x98* bone = &bones->bones[index];
    if (!bone)
        return false;
    UnkMatrix44 m = fn_71000127c8(self->xform.GetMatrix(), bone->matrix);
    UnkCopyMatrix(out, m);
    return true;
}

// NON_MATCHING(minor): ours folds the not-found case to `return false`; the original compares the
// end pointer with itself and null-checks it (cmp + ccmp).
bool fn_71001c1694(UnkBoneOwner* self, const u64* id, UnkMatrix44* out) {
    UnkBoneArray* bones = self->holder->bones;
    fn_710019b9c8(bones);
    UnkBone_0x98* bone = bones->by_id.Find(*id);
    if (bone != bones->by_id.End() && bone) {
        UnkMatrix44 m = fn_71000127c8(self->xform.GetMatrix(), bone->matrix);
        UnkCopyMatrix(out, m);
        return true;
    }
    return false;
}

// The model-space matrix of a bone: the matrices up its chain of parents (a bone starts with its
// parent and a pointer to its matrix), times the owner's scale (fn_7100079138 builds the scale
// matrix): fn_71001c1c70.
struct UnkLinkedBone {
    UnkLinkedBone* parent;
    const UnkMatrix44* matrix;
};

UnkMatrix44 fn_7100079138(f32 x, f32 y, f32 z);  // scale matrix

__attribute__((always_inline)) static inline void SetIdentity(UnkMatrix44* m) {
    // Zeroes everything, then sets the diagonal from the last element.
    for (int i = 0; i < 16; ++i)
        m->m[i] = 0.0f;
    for (int i = 3; i >= 0; --i)
        m->m[i * 5] = 1.0f;
}

bool fn_71001c1c70(UnkBoneOwner* self, u32 index, UnkMatrix44* out) {
    UnkLinkedBone* bone = reinterpret_cast<UnkLinkedBone*>(&self->holder->bones->bones[index]);
    SetIdentity(out);
    for (; bone; bone = bone->parent) {
        UnkMatrix44 m = fn_71000127c8(*bone->matrix, *out);
        UnkCopyMatrix(out, m);
    }
    UnkVec3f s = self->xform.GetScale();
    UnkMatrix44 m = fn_71000127c8(fn_7100079138(s.x, s.y, s.z), *out);
    UnkCopyMatrix(out, m);
    return true;
}

// Sets the owner's scale, unless it is within 0.001 of the current one: the size of the part at
// 0x20 follows the largest component (fn_710021693c) and the holder takes the new matrix
// (fn_71001c1f9c).
void fn_710021693c(UnkSized* self, f32 size);

// By reference, as std::max (by value the compiler orders the loads differently).
__attribute__((always_inline)) static inline const f32& Larger(const f32& a, const f32& b) { return a < b ? b : a; }

__attribute__((always_inline)) static inline f32 Largest(UnkVec3f v) { return Larger(Larger(v.x, v.y), v.z); }

void fn_71001c1f9c(UnkBoneOwner* self, const UnkVec3f* scale) {
    UnkVec3f old = self->xform.GetScale();
    if (__builtin_fabsf(old.x - scale->x) < 0.001f && __builtin_fabsf(old.y - scale->y) < 0.001f &&
        __builtin_fabsf(old.z - scale->z) < 0.001f)
        return;
    f32 old_largest = Largest(self->xform.GetScale());
    self->xform.SetScale(*scale);
    fn_710021693c(&self->sized, self->sized.size * (Largest(self->xform.GetScale()) / old_largest));
    const UnkMatrix44& m = self->xform.GetMatrix();
    UnkCopyMatrix(&self->holder->matrix, m);
}
