// CAIAimAction's unit: samples a bone of the blend space at +0xc0 (a base::animation::CBlendSpace1D)
// from `start` to `end` in steps of `step` (fn_7100b8fb48): each sample is the time and the bone's matrix
// under `parent`, appended to `out`. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU23.h"
#include "types.h"

struct UnkAimBlendSpace_b8fb48 {
    virtual const base::reflection::CType* GetClass();
};

struct UnkAimAction_b8fb48 {
    u8 pad_0x0[0xc0];
    UnkAimBlendSpace_b8fb48* blend;  // 0xc0
};

// A pose of the blend space's skeleton.
struct UnkAimPose_b8fb48 {
    u8 data[0x38];

    UnkAimPose_b8fb48() { fn_7100014d94(this, 0); }
    ~UnkAimPose_b8fb48() { fn_7100014e88(this); }
};

// One sample: the time and the bone's matrix.
struct UnkAimSample_b8fb48 {
    f32 time;
    UnkMatrix44 matrix;

    explicit UnkAimSample_b8fb48(f32 t) : matrix() {
        matrix.m[15] = 1.0f;
        matrix.m[10] = 1.0f;
        matrix.m[5] = 1.0f;
        matrix.m[0] = 1.0f;
        time = t;
    }
};

struct UnkAimSamples_b8fb48 {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
};

void fn_7100467cc4(UnkAimSamples_b8fb48* self, s32 capacity);
void fn_710062fe68(UnkAimSamples_b8fb48* self, const UnkAimSample_b8fb48* sample);
void fn_71000152b8(UnkAimPose_b8fb48* pose, u64 bone);
void fn_710000771c(UnkAimBlendSpace_b8fb48* blend, f32 time, UnkAimPose_b8fb48* pose);

void fn_7100b8fb48(UnkAimAction_b8fb48* self, const UnkMatrix44* parent, const u64* bone, UnkAimSamples_b8fb48* out,
                   f32 start, f32 end, f32 step) {
    out->count = 0;
    fn_7100467cc4(out, static_cast<s32>(__builtin_floorf((end - start) / step)) + 1);
    UnkAimBlendSpace_b8fb48* blend = self->blend;
    if (!blend || !fn_710009d50c(blend->GetClass(), fn_71000062ac()))
        return;
    UnkAimPose_b8fb48 pose;
    fn_71000152b8(&pose, *bone);
    for (f32 t = start; t <= end; t += step) {
        fn_710000771c(blend, t, &pose);
        const UnkMatrix44* m = fn_7100015508(&pose, *bone);
        UnkAimSample_b8fb48 sample(t);
        UnkCopyMatrix(&sample.matrix, fn_71000127c8(*parent, m ? *m : g_unk_7101a1a680));
        fn_710062fe68(out, &sample);
    }
}
