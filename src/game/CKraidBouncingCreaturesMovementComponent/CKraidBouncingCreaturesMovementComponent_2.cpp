// CKraidBouncingCreaturesMovementComponent: fn_71006051ec runs the base step (fn_7100ac3108), keeps
// the object at +0x10 cast to its CCharClassKraidBouncingCreaturesMovementComponent class at +0x5d0,
// sets up the request at +0x248 with the damage tunable's value at +0x27c of g_unk_7172716160
// (fn_71008c1538), sets the word at +0x2fc and returns the base step's answer. Names and layouts
// are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CProjectileMovement.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkKraidCreatureRequest;
struct UnkKraidCreatureArg;

struct UnkKraidBouncingCreaturesMovement2 {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x230];
    UnkKraidCreatureRequest* field_0x248[1];
    u8 pad_0x250[0xac];
    u32 field_0x2fc;
    u8 pad_0x300[0x2d0];
    UnkReflObject* field_0x5d0;
};

extern base::reflection::UnkTunable g_unk_7172716160;
struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);

bool fn_71006051ec(UnkKraidBouncingCreaturesMovement2* self, UnkKraidCreatureArg* arg) {
    bool result = fn_7100ac3108(self, arg);
    UnkReflObject* obj = self->field_0x10;
    self->field_0x5d0 = obj && fn_710009d50c(obj->vf_0x0(), fn_7100602884()) ? obj : nullptr;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self->field_0x248), *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0x27c));
    self->field_0x2fc = 0x4000000;
    return result;
}
