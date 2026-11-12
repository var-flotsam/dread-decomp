// Functions in the CSluggerAcidBallMovementComponent unit. fn_71006c79b4 is a copy of fn_71006051ec
// (CKraidBouncingCreaturesMovementComponent_2.cpp) without the word store. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CProjectileMovement.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkSluggerAcidBallMovement {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x230];
    u8 field_0x248[0x388];
    UnkReflObject* field_0x5d0;  // the object as a CCharClassSluggerAcidBallMovementComponent
};

struct UnkRequest;
extern base::reflection::UnkTunable g_unk_7172716160;
struct UnkKraidBouncingCreaturesMovement2;
struct UnkKraidCreatureArg;

// Runs the base step, keeps the object at +0x10 cast to its class at +0x5d0 and sets up the request
// at +0x248 with the tunable's value at +0x360.
bool fn_71006c79b4(UnkSluggerAcidBallMovement* self, UnkKraidCreatureArg* arg) {
    bool result = fn_7100ac3108(reinterpret_cast<UnkKraidBouncingCreaturesMovement2*>(self), arg);
    UnkReflObject* obj = self->field_0x10;
    self->field_0x5d0 = obj && fn_710009d50c(obj->vf_0x0(), fn_71006c71b4()) ? obj : nullptr;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self->field_0x248),
                  *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0x360));
    return result;
}
