// CAnimationComponent (tools/tu_map.py unit 2720). fn_7101179878: samples the pose of an action at a frame
// into a matrix, either the pose object's own matrix for `key`, or the bone `key` names composed with its
// parents up to the model's root. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkAnimActionSetsOwner;

struct UnkMtx44_b46a2 {
    f32 m[16];
};

struct UnkAnimPose_l179878 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool vf_0x30(u64 key, UnkMatrix44* out, f32 frame);
};

struct UnkAnimClip_l179878 {
    f32 start;   // 0x0
    f32 end;     // 0x4
    f32 base;    // 0x8
    f32 range;   // 0xc
    u8 pad_0x10[0x8];
    UnkAnimPose_l179878* pose;  // 0x18
};

struct UnkBoneName_l179878 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkBone_l179878 {
    UnkBone_l179878* parent;  // 0x0
    u8 pad_0x8[0x8];
    const UnkMatrix44* matrix;  // 0x10
    u8 pad_0x18[0x50];
    UnkBoneName_l179878* name;  // 0x68
    u8 pad_0x70[0x28];
};
static_assert(sizeof(UnkBone_l179878) == 0x98, "UnkBone_l179878 size");

struct UnkBoneMap_l179878 {
    u8 pad_0x0[0x8];
    UnkBone_l179878* values;  // 0x8
    u64* keys;                // 0x10
    u32 count;                // 0x18

    UnkBone_l179878* Find(u64 key) {
        for (u64 i = 0; i < count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return &values[count];
    }
};

struct UnkSkeleton_l179878 {
    u8 pad_0x0[0x80];
    UnkBoneMap_l179878* bones;
};

struct UnkSkeletonHolder_l179878 {
    u8 pad_0x0[0x78];
    UnkSkeleton_l179878* skeleton;
};

struct UnkModel_l179878 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0xc0];
    UnkSkeletonHolder_l179878* holder;  // 0x138
};

struct UnkModelHolder_l179878 {
    u8 pad_0x0[0x50];
    UnkModel_l179878* model;
};

struct UnkActor_l179878 {
    u8 pad_0x0[0x228];
    UnkModelHolder_l179878* model_holder;
};

struct UnkAnimOwner_l179878 {
    u8 pad_0x0[0x30];
    UnkActor_l179878* actor;
};

struct UnkActionClip_l179878 {
    u8 pad_0x0[0x120];
};

struct UnkActionEntry_l179878 {
    u8 pad_0x0[0x80];
    UnkActionClip_l179878* clips;  // 0x80
};

UnkAnimClip_l179878* fn_7100c959bc(UnkActionClip_l179878* clip);

inline void SetIdentity(UnkMatrix44* m) {
    m->m[0] = 1.0f;
    m->m[1] = 0.0f;
    m->m[2] = 0.0f;
    m->m[3] = 0.0f;
    m->m[4] = 0.0f;
    m->m[5] = 1.0f;
    m->m[6] = 0.0f;
    m->m[7] = 0.0f;
    m->m[8] = 0.0f;
    m->m[9] = 0.0f;
    m->m[10] = 1.0f;
    m->m[11] = 0.0f;
    m->m[12] = 0.0f;
    m->m[13] = 0.0f;
    m->m[14] = 0.0f;
    m->m[15] = 1.0f;
}

// Returns whether the pose was found; the header still declares it void.
bool fn_7101179878(UnkAnimActionSetsOwner* anim, void* entry, u32 a, u64 key, UnkMtx44_b46a2* out, u32 b, f32 c) {
    if (entry) {
        UnkAnimOwner_l179878* owner = reinterpret_cast<UnkAnimOwner_l179878*>(anim);
        UnkModel_l179878* model = owner->actor->model_holder->model;
        UnkActionEntry_l179878* action = static_cast<UnkActionEntry_l179878*>(entry);
        UnkAnimClip_l179878* clip = fn_7100c959bc(&action->clips[a]);

        f32 t = (c - clip->start) / (clip->end - clip->start);
        UnkAnimPose_l179878* pose = clip->pose;
        f32 hi = clip->base + clip->range;
        f32 v = clip->base + clip->range * t;
        f32 frame = !(v <= clip->base) ? (v >= hi ? hi : v) : clip->base;
        frame = frame < 1.0f ? frame : 1.0f;

        UnkMatrix44* result = reinterpret_cast<UnkMatrix44*>(out);
        if (b == 1)
            return pose->vf_0x30(key, result, frame);

        UnkBoneMap_l179878* bones = model->holder->skeleton->bones;
        UnkBone_l179878* bone = bones->Find(key);
        if (bone != &bones->values[bones->count] && bone) {
            SetIdentity(result);
            UnkMatrix44 local;
            SetIdentity(&local);
            do {
                UnkMatrix44 m;
                if (pose->vf_0x30(bone->name->key, &local, frame))
                    m = fn_71000127c8(local, *result);
                else
                    m = fn_71000127c8(*bone->matrix, *result);
                UnkCopyMatrix(result, m);
                bone = bone->parent;
            } while (bone);

            if (b == 0) {
                UnkMatrix44 m = fn_71000127c8(fn_710002614c(&model->transform), *result);
                UnkCopyMatrix(result, m);
            }
            return true;
        }
    }
    return false;
}
