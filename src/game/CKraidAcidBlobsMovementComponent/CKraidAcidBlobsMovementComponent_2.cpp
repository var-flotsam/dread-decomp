// CKraidAcidBlobsMovementComponent: fn_710060467c runs the base step (fn_7100ac3108), keeps the
// object at +0x10 cast to its CCharClassKraidAcidBlobsMovementComponent class at +0x5d0, sets up the
// request at +0x248 with the damage tunable's value at +0x274 of g_unk_7172716160 (fn_71008c1538) and
// copies a unit-static id into +0x300, like fn_71006051ec (CKraidBouncingCreaturesMovementComponent_2.cpp).
// It sets the word at +0x2fc only when the argument's object is a CCharClass whose record at +0x98
// holds a given hash at +0x30. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CProjectileMovement.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkKraidBouncingCreaturesMovement2;
struct UnkKraidCreatureArg;
struct UnkRequest;

struct UnkKraidAcidBlobsMovement {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x230];
    UnkRequest* field_0x248[1];
    u8 pad_0x250[0xac];
    u32 field_0x2fc;
    CStrId field_0x300;
    u8 pad_0x308[0x2c8];
    UnkReflObject* field_0x5d0;
};

struct UnkKraidAcidBlobsRecord {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkKraidAcidBlobsCharClass {
    u8 pad_0x0[0x98];
    UnkKraidAcidBlobsRecord* record;
};

struct UnkKraidAcidBlobsArg {
    UnkReflObject* object;
};

extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726f4068 __attribute__((visibility("hidden")));
void fn_71008c1538(UnkRequest* request, f32 value);

bool fn_710060467c(UnkKraidAcidBlobsMovement* self, UnkKraidAcidBlobsArg* arg) {
    bool result = fn_7100ac3108(reinterpret_cast<UnkKraidBouncingCreaturesMovement2*>(self),
                                reinterpret_cast<UnkKraidCreatureArg*>(arg));
    UnkReflObject* obj = self->field_0x10;
    self->field_0x5d0 = obj && fn_710009d50c(obj->vf_0x0(), fn_71006023c0()) ? obj : nullptr;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self->field_0x248),
                  *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0x274));
    self->field_0x300 = g_unk_71726f4068;
    UnkReflObject* other = arg->object;
    if (other && fn_710009d50c(other->vf_0x0(), fn_71011abbd4()) &&
        reinterpret_cast<UnkKraidAcidBlobsCharClass*>(other)->record->hash == 0xd43ab84f3b165b78ULL)
        self->field_0x2fc = 0x4000000;
    return result;
}
