// An Infester AI one-shot (tools/tu_map.py: the CInfesterAIComponent unit): the first time, with the
// owner's animation component (+0x30, then +0x220), runs two animation calls around a lookup of one
// action by key; when found, fills an identity matrix for it and keeps its translation at +0x146e4.
// The flag at g_unk_71726ec411 marks it done. Names are placeholders.
#include "types.h"

struct UnkAnimActionSetsOwner;
struct UnkActorTarget;
struct UnkStruct_7101176ea8;

struct UnkMtx44_b46a2 {
    f32 m[16];
};

struct UnkInfesterOwner_b46a2 {
    u8 pad_0x0[0x220];
    UnkAnimActionSetsOwner* anim;
};

struct UnkInfesterAI_b46a2 {
    u8 pad_0x0[0x30];
    UnkInfesterOwner_b46a2* owner;
    u8 pad_0x38[0x146e4 - 0x38];
    f32 pos[3];
};

extern u8 g_unk_71726ec411 __attribute__((visibility("hidden")));

// Signatures as the other AI units declare them.
void fn_710117662c(UnkActorTarget*, u32, u32, u32, u32, f32);
void* fn_7101176d34(UnkStruct_7101176ea8*, void*);
void fn_7101179878(UnkAnimActionSetsOwner* anim, void* entry, u32 a, u64 key, UnkMtx44_b46a2* out, u32 b, f32 c);
void fn_7101178e08(UnkActorTarget*, u32, u32, u32, f32);

void fn_71005cafa4(UnkInfesterAI_b46a2* self) {
    if (g_unk_71726ec411 & 1) return;
    UnkAnimActionSetsOwner* anim = self->owner->anim;
    if (!anim) return;
    fn_710117662c(reinterpret_cast<UnkActorTarget*>(anim), 6, 0, 0, 0, -1.0f);
    void* entry = fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(anim),
                                reinterpret_cast<void*>(0x8e07665b29a16330ULL));
    if (entry) {
        UnkMtx44_b46a2 mtx = {};
        mtx.m[15] = 1;
        mtx.m[10] = 1;
        mtx.m[5] = 1;
        mtx.m[0] = 1;
        fn_7101179878(anim, entry, 0, 0xf5a5aba2ac513c70, &mtx, 2, 25.0f);
        f32 x = mtx.m[3];
        f32 z = mtx.m[11];
        f32 y = mtx.m[7];
        self->pos[0] = x;
        self->pos[1] = y;
        self->pos[2] = z;
    }
    fn_7101178e08(reinterpret_cast<UnkActorTarget*>(anim), 6, 0, 0, -1.0f);
    g_unk_71726ec411 = 1;
}
