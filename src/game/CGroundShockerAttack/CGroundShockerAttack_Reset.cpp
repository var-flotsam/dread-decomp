// CGroundShockerAttack: its reset. The attack base's reset (fn_71003609b8), the word at +0x30
// cleared, the listener at +0x40 unregistered from its owner's target and its owner dropped, the
// two floats at +0x38 set to -1, then fn_71005a50ec on the actor's AI component when that is a
// CGroundShockerAIComponent. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAttack.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkGroundShockerListener {
    void* vtable;
    u64 field_0x8;
    u64 field_0x10;
    UnkWeakHandle* owner;  // 0x18
};

struct UnkGroundShockerAttack : UnkAttackBase {
    u8 pad_0x20[0x10];
    u64 field_0x30;
    f32 field_0x38;
    f32 field_0x3c;
    UnkGroundShockerListener listener;  // 0x40
};

struct UnkGroundShockerActor {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

void fn_71005a50ec(UnkReflObject* ai);

void fn_71005a0bc0(UnkGroundShockerAttack* self) {
    fn_71003609b8(self);
    self->field_0x30 = 0;
    if (UnkWeakHandle* owner = self->listener.owner) {
        if (owner->target) {
            fn_710117b3f0(owner->target, &self->listener);
            owner = self->listener.owner;
            if (!owner)
                goto done;
        }
        if (--owner->ref_count == 0)
            owner->vf_0x10();
        self->listener.owner = nullptr;
    }
done:
    self->field_0x38 = -1.0f;
    self->field_0x3c = -1.0f;
    UnkReflObject* ai = reinterpret_cast<UnkGroundShockerActor*>(self->field_0x8)->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71005a4004()))
        fn_71005a50ec(ai);
}
