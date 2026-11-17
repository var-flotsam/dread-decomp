// UnkTU811 (CEmmyAIComponent's unit): whether the owner is out of the player's view and far enough from its
// target, told to the game manager's watchers as the opposite flag. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU39.h"
#include "game/UnkTU811.h"
#include "game/UnkVec3Bits.h"
#include "types.h"

#include <math.h>

struct UnkBossOwner_b49;
bool fn_7100d7b184(UnkBossOwner_b49* owner);
struct UnkScaledRecord;
void fn_710021688c(f32 scale, UnkScaledRecord* self, const u32* vec);
struct UnkFrustumView;
struct UnkBoundSphere;
// The original passes a third argument (null) that the definition in base/spatial/UnkFrustumCull.cpp lacks.
s32 fn_71001bab30(const UnkFrustumView* self, UnkBoundSphere* sphere, void* extra);

struct UnkPlace;

struct UnkCamera_l54bcb8 {
    u8 pad_0x0[0x130];
    UnkFrustumView* frustum;  // 0x130
};

struct UnkPlayer_l54bcb8 {
    u8 pad_0x0[0x40];
    UnkCamera_l54bcb8* camera;  // 0x40
};

// A bounding sphere built on the stack (fn_710021688c builds it, fn_7100216934 tears it down).
struct alignas(8) UnkSphere_l54bcb8 {
    u8 data[0x50];
};

struct UnkManagerView_l54bcb8 {
    u8 pad_0x0[0x1da8];
    u8 field_0x1da8[1];  // a callback set told the flag
};

struct UnkEmmyTuning_l54bcb8 {
    u8 pad_0x0[0x90];
    f32 farNear;  // 0x90
    f32 farAway;  // 0x94
    bool checkAxes;  // 0x98
};

struct UnkEmmyContext_l54bcb8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual UnkEmmyTuning_l54bcb8* vf_0xd8();  // the tuning block
};

struct UnkEmmyRanges_l54bcb8 {
    f32 rangeView;  // 0x0
    u8 pad_0x4[0x70 - 0x4];
    f32 rangeB;  // 0x70
    u8 pad_0x74[0xe0 - 0x74];
    f32 rangeC;  // 0xe0
    u8 pad_0xe4[0x150 - 0xe4];
    f32 rangeA;  // 0x150
};

struct UnkEmmyAI_l54bcb8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x149b0 - 0x38];
    UnkEmmyRanges_l54bcb8 ranges;  // 0x149b0
    u8 pad_0x14b04[0x27720 - 0x14b04];
    bool nearMode;  // 0x27720
    u8 pad_0x27721[0x29ee0 - 0x27721];
    UnkEmmyContext_l54bcb8* context;  // 0x29ee0
};
static_assert(__builtin_offsetof(UnkEmmyAI_l54bcb8, ranges) == 0x149b0, "UnkEmmyAI_l54bcb8 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l54bcb8, nearMode) == 0x27720, "UnkEmmyAI_l54bcb8 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l54bcb8, context) == 0x29ee0, "UnkEmmyAI_l54bcb8 layout");

// Where the owner's target stands: its place when it has one, else its transform; zero without a target.
static inline void GetTargetPosition(UnkActor* owner, f32* x, f32* y, f32* z) {
    UnkActor* target = fn_7100d7c990(owner);
    if (!target) {
        *x = 0.0f;
        *y = 0.0f;
        *z = 0.0f;
        return;
    }
    if (target->field_0x1f8) {
        UnkVec3Bits place = fn_7100979a60(static_cast<UnkPlace*>(target->field_0x1f8));
        *x = place.x;
        *y = place.y;
        *z = place.z;
        return;
    }
    *x = target->field_0x44;
    *y = target->field_0x54;
    *z = target->field_0x64;
}

// Whether the player's camera sees the owner within the current detection range.
static inline bool IsSeen(UnkEmmyAI_l54bcb8* self, UnkPlayer_l54bcb8* player) {
    f32 range;
    if (fn_710054ad2c(reinterpret_cast<UnkEmmyAIView*>(self))) {
        range = self->ranges.rangeView;
    } else if (fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self))) {
        range = self->ranges.rangeA;
    } else if (fn_710054aecc(reinterpret_cast<UnkEmmyAI2_b62a3*>(self))) {
        range = self->ranges.rangeB;
    } else if (fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self))) {
        range = self->ranges.rangeC;
    } else {
        range = 0.0f;
    }
    UnkSphere_l54bcb8 sphere;
    UnkActor* owner = self->owner;
    UnkVec3Bits center = UnkVec3Bits::CopyBits(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    fn_710021688c(range, reinterpret_cast<UnkScaledRecord*>(&sphere), reinterpret_cast<const u32*>(&center));
    s32 side = fn_71001bab30(player->camera->frustum, reinterpret_cast<UnkBoundSphere*>(&sphere), nullptr);
    fn_7100216934(reinterpret_cast<char*>(&sphere));
    return side != 1;
}

static inline bool IsOutOfView(UnkEmmyAI_l54bcb8* self) {
    if (!fn_7100d7b184(reinterpret_cast<UnkBossOwner_b49*>(self->owner)))
        return false;
    UnkPlayer_l54bcb8* player = static_cast<UnkPlayer_l54bcb8*>(fn_7100defcec(g_unk_7101d47310));
    if (player && IsSeen(self, player))
        return false;
    if (!fn_7100d7c990(self->owner))
        return true;

    bool nearMode = self->nearMode;
    UnkEmmyTuning_l54bcb8* tuning = self->context->vf_0xd8();
    f32 farDistance = nearMode ? tuning->farNear : tuning->farAway;
    UnkActor* owner = self->owner;
    f32 ox = owner->field_0x44;
    f32 oy = owner->field_0x54;
    f32 oz = owner->field_0x64;
    f32 tx, ty, tz;
    GetTargetPosition(owner, &tx, &ty, &tz);
    f32 dx = ox - tx;
    f32 dy = oy - ty;
    f32 dz = oz - tz;
    if (!(sqrtf(dz * dz + (dy * dy + dx * dx)) > farDistance))
        return false;
    return !self->context->vf_0xd8()->checkAxes || (fabsf(dx) > 2700.0f && fabsf(dy) > 1500.0f);
}

bool fn_710054bcb8(UnkEmmyAI_l54bcb8* self) {
    bool outOfView = IsOutOfView(self);
    bool visible = !outOfView;
    fn_71000303c0(reinterpret_cast<base::UnkDeferredSet<base::UnkDeferredCallback<bool>>*>(
                      reinterpret_cast<UnkManagerView_l54bcb8*>(g_unk_7101d47310)->field_0x1da8),
                  &visible);
    return outOfView;
}
