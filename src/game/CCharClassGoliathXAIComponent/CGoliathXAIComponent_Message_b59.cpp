// The CGoliathXAIComponent unit. fn_7100588858 handles a message: after the base handler
// (fn_7100590fc4), a message without flag 0x8000 answers 2 when none of the flags 0x2800041c is set.
// Otherwise, when the owner (+0x30) has its animation (+0x220), the object at +0x278 and an actor to act
// against: calls the object's vf_0x178; when the target sits on the other side (sign of its position x
// against the owner's facing, matrix element 2) and the animation has a
// CCharClassTweakShinesparkImpactActionTrack, sends it a one under id 0x8735a75cd098883a; else sends a
// one under an id picked by the component's value (fn_71010fc68c) against |+0x14cc0| and turns the owner
// to face the target (vf_0xd8 with the angle +-pi/2). Descriptors as UnkSpitSend
// (src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2702.h"
#include "types.h"

// As in src/game/CKraidBouncingCreaturesMovementComponent/CKraidBouncingCreaturesMovementComponent_3.cpp.
struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkGoliathAI_b58a3;

struct UnkGoliathXRotation_b59a3 {
    f32 x, y, z;
};

struct UnkGoliathXOwner_b59a3 {
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
    virtual void vf_0xd8(const UnkGoliathXRotation_b59a3* rotation, bool now);
};

struct UnkGoliathXObject_b59a3 {
    void** vtable;
    void vf_0x178() { reinterpret_cast<void (*)(UnkGoliathXObject_b59a3*)>(vtable[0x178 / 8])(this); }
};

struct UnkGoliathXMessage_b59a3 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

struct UnkGoliathXAI_b59a3 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14cc0 - 0x38];
    f32 field_0x14cc0;
};

u32 fn_7100590fc4(UnkGoliathXAI_b59a3* self, UnkGoliathXMessage_b59a3* message);

static inline f32 Sign_b59a3(f32 x) {
    return (f32)((x > 0.0f) - (x < 0.0f));
}

static inline const UnkMatrix44& Xform_b59a3(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

u32 fn_7100588858(UnkGoliathXAI_b59a3* self, UnkGoliathXMessage_b59a3* message) {
    u32 result = fn_7100590fc4(self, message);
    if (!(message->flags & 0x8000)) {
        if (!(message->flags & 0x2800041c))
            result = 2;
        return result;
    }
    UnkAnimationComponent* target = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    auto* object = *reinterpret_cast<UnkGoliathXObject_b59a3**>(reinterpret_cast<char*>(self->owner) + 0x278);
    UnkActor* other = fn_7100d7c990(self->owner);
    UnkGoliathXOwner_b59a3* owner;
    // The gotos skip the declarations below, so they get no lifetime markers and keep slots of their own.
    if (!target || !object || !other)
        return result;
    object->vf_0x178();
    {
        f32 side = Sign_b59a3(fn_7100d7be08(other).x);
        if (side != Sign_b59a3(Xform_b59a3(self->owner).m[2]) && fn_710117b348(target, fn_7100c624f8()))
            goto impact;
    }
    goto turn;
    base::reflection::UnkReflTypeIdArg mode1;
    base::reflection::UnkReflTypeIdArg mask1;
    f32 value;
    u32 mode2;
    base::reflection::UnkReflTypeIdArg mask2;
impact:
    mode1.id = 1;
    mask1.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0x8735a75cd098883a, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740,
                  &mask1);
    return result;
turn:
    fn_71010fc68c(reinterpret_cast<UnkGoliathAI_b58a3*>(self), &value);
    {
        u64 hash = !(value >= __builtin_fabsf(self->field_0x14cc0)) ? 0x9b214780df6c8f00 : 0xbe901ff6f5cba1f7;
        mode2 = 1;
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash, 0xf4061ff25a2163d6, reinterpret_cast<base::reflection::UnkReflTypeIdArg*>(&mode2), &g_unk_710164b958, &g_unk_7172769740, &mask2);
    }
    owner = reinterpret_cast<UnkGoliathXOwner_b59a3*>(self->owner);
    {
        f32 facing = Xform_b59a3(other).m[2];
        UnkGoliathXRotation_b59a3 rotation;
        rotation.x = 0.0f;
        rotation.z = 0.0f;
        rotation.y = facing >= 0.0f ? 1.5707964f : -1.5707964f;
        owner->vf_0xd8(&rotation, true);
    }
    return result;
}
