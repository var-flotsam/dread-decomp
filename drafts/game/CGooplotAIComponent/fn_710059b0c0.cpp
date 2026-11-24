// CGooplotAIComponent unit: sets the Gooplot moving along a direction. The velocity (+0x146cc) starts
// from the movement's (+0x1d8) and takes the launch speed on each axis the direction points; then it
// sends the movement a parameter block, the owner's sub-object (+0x220) one of two hashes and sets the
// two flags on the component at +0x228. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/math/CVector.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CMovementComponent.h"
#include "types.h"

using base::math::CVector3D;

struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
struct UnkEffectSink;
struct UnkFlagTarget;
extern char g_unk_710164a040[];
extern char g_unk_710164a048[];
extern f32 g_unk_7101a4d0c0;
extern u64 g_unk_710164a050;
extern u64 g_unk_710164a058;

// The block handed to the owner's +0x218 (fn_71010c7e6c); the vtable tells the kinds apart.
struct UnkEffectParams2_b0117 {
    void** vtable;
    f32 field_0x8;
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
};

struct UnkGooplotMovement_l59b0c0 {
    u8 pad_0x0[0x1d8];
    CVector3D velocity;
};

struct UnkGooplotOwner_l59b0c0 {
    u8 pad_0x0[0x218];
    UnkGooplotMovement_l59b0c0* movement;
    void* field_0x220;
    UnkFlagTarget* field_0x228;
};

struct UnkGooplotAI_l59b0c0 {
    u8 pad_0x0[0x30];
    UnkGooplotOwner_l59b0c0* owner;
    u8 pad_0x38[0x146cc - 0x38];
    CVector3D velocity;
    u8 pad_0x146d8[0x14870 - 0x146d8];
    bool field_0x14870;
    u8 pad_0x14871[3];
    CVector3D field_0x14874;
};

// -1, 0 or 1 for the sign of v.
static inline f32 Sign_l59b0c0(f32 v) {
    return (f32)((s32)(v > 0.0f) - (s32)(v < 0.0f));
}

static inline void SetUp_l59b0c0(CVector3D& v) {
    v.x = 0.0f;
    v.y = 1.0f;
    v.z = 0.0f;
}

void fn_710059b0c0(UnkGooplotAI_l59b0c0* self, const CVector3D* dir, bool keepVertical, bool vertical) {
    UnkGooplotMovement_l59b0c0* movement = self->owner->movement;
    self->velocity.x = movement->velocity.x;
    self->velocity.y = movement->velocity.y;
    self->velocity.z = movement->velocity.z;
    self->velocity.x = g_unk_7101a4d0c0 * Sign_l59b0c0(dir->x);
    if (keepVertical) {
        self->velocity.y = __builtin_fabsf(dir->y) * self->velocity.y;
    } else if (vertical) {
        self->velocity.y = g_unk_7101a4d0c0 * Sign_l59b0c0(dir->y);
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(movement),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(&self->velocity), true);
    }
    SetUp_l59b0c0(self->field_0x14874);

    UnkEffectParams2_b0117 params;
    params.field_0xc = 0;
    params.field_0x14 = 0;
    __builtin_memset(params.field_0x18, 0, 13);
    __builtin_memset(params.field_0x28, 0, 13);
    params.vtable = g_unk_7101a2e1c0 + 2;
    params.field_0x38 = 2;
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(movement), &params, 1);

    void* target = self->owner->field_0x220;
    u64 hash = self->field_0x14870 ? g_unk_710164a050 : g_unk_710164a058;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 2;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);

    if (UnkFlagTarget* comp = self->owner->field_0x228) {
        fn_7101172764(comp, g_unk_710164a040);
        fn_7101172780(comp, g_unk_710164a048);
    }
    SetUp_l59b0c0(self->field_0x14874);
}
