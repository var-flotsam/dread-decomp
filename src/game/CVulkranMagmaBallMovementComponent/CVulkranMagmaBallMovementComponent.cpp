// CVulkranMagmaBallMovementComponent: fn_71006df684 runs the base step (fn_7100ac3108) and keeps the
// object at +0x10 cast to its CCharClassVulkranMagmaBallMovementComponent class at +0x5d0, like
// fn_71006051ec (CKraidBouncingCreaturesMovementComponent_2.cpp); when the cast holds it moves an id
// built from the class's string at +0x68 into +0x300. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkKraidBouncingCreaturesMovement2;
struct UnkKraidCreatureArg;

struct UnkVulkranMagmaBallMovement {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
    u8 pad_0x18[0x2e8];
    CStrId field_0x300;
    u8 pad_0x308[0x2c8];
    UnkReflObject* field_0x5d0;
};

// An id built from a string out of line, released like CStrId.
struct UnkVulkranBuiltId {
    union {
        CStrId id;
    };

    explicit UnkVulkranBuiltId(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkVulkranBuiltId() { id.~CStrId(); }
};

bool fn_7100ac3108(UnkKraidBouncingCreaturesMovement2* self, UnkKraidCreatureArg* arg);

bool fn_71006df684(UnkVulkranMagmaBallMovement* self, UnkKraidCreatureArg* arg) {
    bool result = fn_7100ac3108(reinterpret_cast<UnkKraidBouncingCreaturesMovement2*>(self), arg);
    UnkReflObject* obj = self->field_0x10;
    if (!obj || !fn_710009d50c(obj->vf_0x0(), fn_71006defd8())) {
        self->field_0x5d0 = nullptr;
        return result;
    }
    self->field_0x5d0 = obj;
    UnkVulkranBuiltId built(*reinterpret_cast<const CRntString*>(reinterpret_cast<const char*>(obj) + 0x68));
    self->field_0x300.ReleaseAndTake(built.id);
    return result;
}
