// CSamusMovement's unit (unit 1501): when the character class has a CCharClassForceRotationOnActionTrack,
// turns the actor a quarter turn towards the side it moves to, plays that side's action and restarts the
// animation. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT

// The CCharClassForceRotationOnActionTrack record: the action for each side.
struct UnkForceRotationTrack_l9eaa88 {
    u8 pad_0x0[0x18];
    CStrId left;   // +0x18
    CStrId right;  // +0x20
};

struct UnkTurnActor_l9eaa88;
struct UnkTurnActorVtbl_l9eaa88 {
    void* slots[0x118 / 8];
    void (*SetTransform)(UnkTurnActor_l9eaa88* self, UnkXform* xform, base::reflection::UnkReflTypeIdArg* how);
};
struct UnkTurnActor_l9eaa88 {
    UnkTurnActorVtbl_l9eaa88* vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // +0x38
    u8 pad_0x94[0x18c];
    void* animation;  // +0x220
    u8 pad_0x228[0x198];
    bool field_0x3c0;
};
static_assert(__builtin_offsetof(UnkTurnActor_l9eaa88, animation) == 0x220, "");
static_assert(__builtin_offsetof(UnkTurnActor_l9eaa88, field_0x3c0) == 0x3c0, "");

struct UnkSamusMovement_l9eaa88 {
    u8 pad_0x0[0x30];
    UnkTurnActor_l9eaa88* actor;  // +0x30
    u8 pad_0x38[0x6b0];
    f32 field_0x6e8;
};

static inline const UnkForceRotationTrack_l9eaa88* Track(const UnkTrackQuery& query) {
    return reinterpret_cast<const UnkForceRotationTrack_l9eaa88*>(query.result);
}

// Turns the actor to `yaw`.
static inline void Face(UnkSamusMovement_l9eaa88* self, f32 yaw) {
    UnkTurnActor_l9eaa88* actor = self->actor;
    UnkXform* xform = &actor->xform;
    xform->SetAngle(UnkVec3f(0.0f, yaw, 0.0f));
    base::reflection::UnkReflTypeIdArg how;
    how.id = 2;
    actor->vtable->SetTransform(actor, xform, &how);
    actor->field_0x3c0 = true;
}

// Plays `action` when there is one.
static inline void Play(UnkSamusMovement_l9eaa88* self, const CStrId& action) {
    if (action.data != &g_unk_7101d47108->empty)
        base::reflection::refl_EmitU32(self->actor->animation, action.data->Key(), 0xf4061ff25a2163d6ull, 0,
                                       &g_unk_710164b968, &g_unk_7172769740);
}

bool fn_71009eaa88(void* movement) {
    UnkSamusMovement_l9eaa88* self = static_cast<UnkSamusMovement_l9eaa88*>(movement);
    UnkTrackQuery query = {fn_7100c1ee78(), nullptr, -1.0f};
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->actor->animation);
    if (!animation)
        return false;
    fn_710117b08c(animation, &query);
    if (!query.result)
        return false;
    if (self->field_0x6e8 >= 0.0f) {
        Face(self, 1.57f);
        Play(self, Track(query)->right);
    } else {
        Face(self, -1.57f);
        Play(self, Track(query)->left);
    }
    fn_7101176a88(static_cast<UnkActorTarget*>(self->actor->animation), true);
    return true;
}
