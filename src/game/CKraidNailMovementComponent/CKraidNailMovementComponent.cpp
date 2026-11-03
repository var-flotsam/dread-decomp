// CKraidNailMovementComponent: fn_7100606404 runs the base step (fn_7100ac3108), keeps the
// object at +0x10 cast to its CCharClassKraidNailMovementComponent class at +0x5d8, sets up
// the request at +0x248 with the damage tunable's value at +0x290 of g_unk_7172716160 (fn_71008c1538),
// copies a unit-static id into +0x300 and sets the word at +0x2fc, like fn_71006051ec
// (CKraidBouncingCreaturesMovementComponent_2.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkKraidBouncingCreaturesMovement2;
struct UnkKraidCreatureArg;
struct UnkRequest;

struct UnkKraidNailMovement {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x230];
    UnkRequest* field_0x248[1];
    u8 pad_0x250[0xac];
    u32 field_0x2fc;
    CStrId field_0x300;
    u8 pad_0x308[0x2d0];
    UnkReflObject* field_0x5d8;
};

extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726f4338 __attribute__((visibility("hidden")));
bool fn_7100ac3108(UnkKraidBouncingCreaturesMovement2* self, UnkKraidCreatureArg* arg);
void fn_71008c1538(UnkRequest* request, f32 value);

bool fn_7100606404(UnkKraidNailMovement* self, UnkKraidCreatureArg* arg) {
    bool result = fn_7100ac3108(reinterpret_cast<UnkKraidBouncingCreaturesMovement2*>(self), arg);
    UnkReflObject* obj = self->field_0x10;
    self->field_0x5d8 = obj && fn_710009d50c(obj->vf_0x0(), fn_7100602d44()) ? obj : nullptr;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self->field_0x248),
                  *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0x290));
    self->field_0x300 = g_unk_71726f4338;
    self->field_0x2fc = 0x4000000;
    return result;
}
