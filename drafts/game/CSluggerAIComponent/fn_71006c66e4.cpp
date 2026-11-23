// CSluggerAIComponent unit (tools/tu_map.py): after the AI component's fn_7101105c94, when `actor` is the
// owner's (+0x30) actor (fn_7100d7c990), the owner's animation component (+0x220) plays none of six track
// classes and the owner is not frozen (+0x2c8, fn_71007f9f20), sends the animation component one of four
// hashes by side (whether `actor` is behind the owner) and by whether the enemy movement's current id
// plays (fn_710117ae38); when the actor is behind and the send took, hands the owner's +0x298 a facing
// query (fn_71006f4b0c). Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CFrozenComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkWarLotusAI;

struct UnkSluggerDir_l6c66e4 {
    u32 x, y, z;
};

// The two facings the query turns the owner to.
__attribute__((visibility("hidden"))) extern const UnkSluggerDir_l6c66e4 g_unk_710164a768;
__attribute__((visibility("hidden"))) extern const UnkSluggerDir_l6c66e4 g_unk_710164a774;

struct UnkSluggerQuery_l6c66e4 {
    void** vtable;
    UnkSluggerDir_l6c66e4 dir;
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38[8];

    UnkSluggerQuery_l6c66e4() {
        vtable = g_unk_7101a24b58 + 2;
        __builtin_memset(&dir, 0, 13);
        field_0x1c = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = 0;
        field_0x34 = 0;
    }
};

struct UnkSluggerAction_l6c66e4 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkSluggerMovement_l6c66e4 : UnkReflObject {
    u8 pad_0x8[0x88];
    UnkSluggerAction_l6c66e4* action;  // 0x90
};

struct UnkSluggerMovementSlot_l6c66e4 {
    u8 pad_0x0[0x10];
    UnkSluggerMovement_l6c66e4* movement;
};

struct UnkSluggerOwner_l6c66e4 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38; m[3], m[7], m[11] are the position
    u8 pad_0x78[0x1a0];
    UnkSluggerMovementSlot_l6c66e4* movement_slot;  // 0x218
    UnkAnimationComponent* animation;               // 0x220
    u8 pad_0x228[0x70];
    void* facing;  // 0x298
    u8 pad_0x2a0[0x28];
    void* frozen;  // 0x2c8
};
static_assert(__builtin_offsetof(UnkSluggerOwner_l6c66e4, frozen) == 0x2c8, "layout");

struct UnkSluggerAI_l6c66e4 {
    u8 pad_0x0[0x30];
    UnkSluggerOwner_l6c66e4* owner;
};

static inline UnkSluggerMovement_l6c66e4* MovementOf(UnkSluggerOwner_l6c66e4* owner) {
    if (!owner->movement_slot)
        return nullptr;
    UnkSluggerMovement_l6c66e4* movement = owner->movement_slot->movement;
    if (!movement)
        return nullptr;
    return fn_710009d50c(movement->vf_0x0(), fn_710095629c()) ? movement : nullptr;
}

struct UnkSluggerPos_l6c66e4 {
    f32 x, y, z;
};

static inline UnkSluggerPos_l6c66e4 operator-(const UnkSluggerPos_l6c66e4& a, const UnkSluggerPos_l6c66e4& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline UnkSluggerPos_l6c66e4 Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline UnkSluggerPos_l6c66e4 Pos(const UnkSluggerOwner_l6c66e4* owner) {
    return {owner->transform.m[3], owner->transform.m[7], owner->transform.m[11]};
}

void fn_71006c66e4(UnkSluggerAI_l6c66e4* self, UnkActor* actor) {
    fn_7101105c94(reinterpret_cast<UnkWarLotusAI*>(self));
    UnkSluggerOwner_l6c66e4* owner = self->owner;
    UnkAnimationComponent* animation = owner ? owner->animation : nullptr;
    UnkSluggerMovement_l6c66e4* movement = owner ? MovementOf(owner) : nullptr;
    void* frozen = self->owner ? self->owner->frozen : nullptr;
    if (!actor)
        return;
    if (fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)) != actor)
        return;
    if (animation) {
        if (fn_710117b348(animation, fn_7100d1b068()))
            return;
        if (fn_710117b348(animation, fn_7100cee4b8()))
            return;
        if (fn_710117b348(animation, fn_7100d2b3a8()))
            return;
        if (fn_710117b348(animation, fn_7100ceda9c()))
            return;
        if (fn_710117b348(animation, fn_7100d253f8()))
            return;
        if (fn_710117b348(animation, fn_7100cf3508()))
            return;
    }
    bool active = frozen ? !fn_71007f9f20(frozen) : true;
    if (!animation || !active)
        return;

    bool playing = movement ? fn_710117ae38(animation, movement->action->id, false) : false;
    UnkSluggerOwner_l6c66e4* o = self->owner;
    UnkSluggerPos_l6c66e4 d = Pos(actor) - Pos(o);
    const UnkMatrix44& m = fn_710002614c(&o->transform);
    f32 ahead = d.x * m.m[2] + d.y * m.m[6] + d.z * m.m[10];
    u64 hash = playing ? (ahead < -0.001f ? 0x8704b8cc4ce83b2cull : 0x6559ff66e3642724ull)
                       : (ahead < -0.001f ? 0xa5e3339d6e2192ceull : 0xc712109584240aa7ull);
    // Sent with mode 0, as UnkSpitSend does.
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    bool sent = fn_7101176690(animation, hash, 0xf4061ff25a2163d6ull, &mode, &g_unk_710164b958,
                              &g_unk_7172769740, &mask);
    if (!(ahead < -0.001f) || !sent)
        return;

    UnkSluggerOwner_l6c66e4* owner2 = self->owner;
    void* facing = owner2->facing;
    if (!facing)
        return;
    UnkSluggerQuery_l6c66e4 query;
    query.dir = fn_710002614c(&owner2->transform).m[2] >= 0.0f ? g_unk_710164a768 : g_unk_710164a774;
    query.field_0x14 = 1;
    fn_71006f4b0c(facing, &query, 0);
}