// The world matrix of one bone of a skinned model, if it exists: the owner's transform matrix
// (UnkXform at +0x80, include/game/UnkXformNode.h) times the bone's matrix (fn_71000127c8). The
// bones (0x98-byte entries keyed by u64 ids, in the skeleton reached through +0x78 and +0x80) are
// updated first (fn_710019b9c8). By index (fn_71001c1b70) or by id (fn_71001c1694).
#include "base/UnkCloneCalls.h"
#include "game/UnkXformNode.h"

struct UnkBone_0x98 {
    UnkBone_0x98* parent;
    u8 pad_0x8[0x8];
    const UnkMatrix44* matrix_0x10;
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
    u8 pad_0x20[0x10];
    bool field_0x30;
};

struct UnkBoneArrayHolder {
    u8 pad_0x0[0x80];
    UnkBoneArray* bones;
    UnkMatrix44 matrix;
};

// Updated with a size (fn_710021693c) and a position (fn_7100216978).
struct UnkSized {
    u8 pad_0x0[0x8];
    f32 size;
    UnkVec3f offset;  // a point in the owner's space
};

struct UnkBoneOwner {
    u8 pad_0x0[0x20];
    UnkSized sized;
    u8 pad_0x38[0x40];
    UnkBoneArrayHolder* holder;
    UnkXform xform;
    u8 pad_0xdc[0x14];
    u8 matrices_by_id[0x28];  // fn_7100015508 looks a matrix up by id
    u8 pad_0x118[0x241];
    bool field_0x359;
};
static_assert(offsetof(UnkBoneOwner, matrices_by_id) == 0xf0, "layout");

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

// The bone with an id, or null (the lookup is written as a null-or-pointer result).
__attribute__((always_inline)) static inline UnkBone_0x98* FindBone(UnkBoneArray* bones, const u64& id) {
    UnkBone_0x98* p = bones->by_id.Find(id);
    return p == bones->by_id.End() ? nullptr : p;
}

bool fn_71001c1694(UnkBoneOwner* self, const u64* id, UnkMatrix44* out) {
    UnkBoneArray* bones = self->holder->bones;
    fn_710019b9c8(bones);
    UnkBone_0x98* bone = FindBone(bones, *id);
    if (bone) {
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

// The same by id (fn_71001c1920).
bool fn_71001c1920(UnkBoneOwner* self, const u64* id, UnkMatrix44* out) {
    UnkBoneArray* bones = self->holder->bones;
    UnkBone_0x98* found = FindBone(bones, *id);
    if (!found)
        return false;
    UnkLinkedBone* bone = reinterpret_cast<UnkLinkedBone*>(found);
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

// Stores a bone's matrix relative to its parent, from one relative to the owner: the owner's matrix
// times the parent's, inverted in place (fn_710007839c), times the given one. Kept in the owner's
// matrices by id (fn_7100015508) when the id has one; the skeleton is then marked for update
// (fn_71001c1514).
void fn_710007839c(UnkMatrix44* m);
UnkMatrix44* fn_7100015508(void* matrices, u64 id);

bool fn_71001c1514(UnkBoneOwner* self, const u64* id, const UnkMatrix44* m) {
    UnkBoneArray* bones = self->holder->bones;
    UnkBone_0x98* bone = FindBone(bones, *id);
    if (!bone)
        return false;
    UnkMatrix44 inv = fn_71000127c8(self->xform.GetMatrix(), bone->parent->matrix);
    fn_710007839c(&inv);
    UnkMatrix44 local = fn_71000127c8(inv, *m);
    if (UnkMatrix44* stored = fn_7100015508(self->matrices_by_id, *id))
        UnkCopyMatrix(stored, local);
    bones->field_0x30 = false;
    return true;
}

// A bone's own matrix, or the one it points to at 0x10, by id after the update (fn_71001c17e4,
// fn_71001c1e5c).
bool fn_71001c17e4(UnkBoneOwner* self, const u64* id, UnkMatrix44* out) {
    UnkBoneArray* bones = self->holder->bones;
    fn_710019b9c8(bones);
    UnkBone_0x98* bone = FindBone(bones, *id);
    if (!bone)
        return false;
    UnkCopyMatrix(out, bone->matrix);
    return true;
}

bool fn_71001c1e5c(UnkBoneOwner* self, const u64* id, UnkMatrix44* out) {
    UnkBoneArray* bones = self->holder->bones;
    fn_710019b9c8(bones);
    UnkBone_0x98* bone = FindBone(bones, *id);
    if (!bone)
        return false;
    UnkCopyMatrix(out, *bone->matrix_0x10);
    return true;
}

// Sets the owner's matrix when it differs (the matrix becomes valid and a global mask of flags is
// cleared): the part at 0x20 takes its point moved by the new matrix (fn_7100077c70,
// fn_7100216978) and a size scaled as the largest scale component, and the holder the new matrix
// (fn_71001c21d4).
extern u32 g_unk_716e554858 __attribute__((visibility("hidden")));
UnkVec3f fn_7100077c70(const UnkMatrix44& m, const UnkVec3f& p);
void fn_7100216978(UnkSized* self, const UnkVec3f* position);

void fn_71001c21d4(UnkBoneOwner* self, const UnkMatrix44* m) {
    if (__builtin_memcmp(m, &self->xform.GetMatrix(), sizeof(UnkMatrix44)) == 0)
        return;
    f32 old_largest = Largest(self->xform.GetScale());
    for (int i = 0; i < 16; ++i)
        (&self->xform.m[0][0])[i] = m->m[i];
    self->xform.flags = (self->xform.flags | 8) & ~g_unk_716e554858;
    UnkSized* sized = &self->sized;
    {
        UnkVec3f p = fn_7100077c70(self->xform.GetMatrix(), sized->offset);
        fn_7100216978(sized, &p);
    }
    f32 size = sized->size;
    fn_710021693c(sized, size * (Largest(self->xform.GetScale()) / old_largest));
    const UnkMatrix44& n = self->xform.GetMatrix();
    UnkCopyMatrix(&self->holder->matrix, n);
    self->field_0x359 = false;
}

// The same from a whole transform, copied when it is another one (fn_71001c2480).
void fn_71001c2480(UnkBoneOwner* self, UnkXform* x) {
    if (__builtin_memcmp(&x->GetMatrix(), &self->xform.GetMatrix(), sizeof(UnkMatrix44)) == 0)
        return;
    f32 old_largest = Largest(self->xform.GetScale());
    if (&self->xform != x)
        new (&self->xform) UnkXform(*x);
    UnkSized* sized = &self->sized;
    {
        UnkVec3f p = fn_7100077c70(self->xform.GetMatrix(), sized->offset);
        fn_7100216978(sized, &p);
    }
    f32 size = sized->size;
    fn_710021693c(sized, size * (Largest(self->xform.GetScale()) / old_largest));
    const UnkMatrix44& n = self->xform.GetMatrix();
    UnkCopyMatrix(&self->holder->matrix, n);
}
