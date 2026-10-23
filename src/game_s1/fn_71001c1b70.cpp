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
};

struct UnkBoneOwner {
    u8 pad_0x0[0x78];
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
