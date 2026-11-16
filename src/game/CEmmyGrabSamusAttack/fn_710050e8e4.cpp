// Frees an object's array of string ids while holding its lock, then frees its other
// array (whose elements need no destruction).
#include "base/UnkHashStream.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

// The 0x38-byte object at +0x28 is the hash stream (base/UnkHashStream.h); fn_710007c0e4 destroys it.

struct UnkStruct_710050e8e4 {
    base::global::CRntVector<u64> field_0x0;
    UnkHashStream_0x38 field_0x28;
    base::global::CRntVector<UnkStrIdTwoStep> field_0x60;
};

void fn_710050e8e4(UnkStruct_710050e8e4* self) {
    fn_710007c144(&self->field_0x28);
    self->field_0x60.Free();
    fn_710007c0e4(&self->field_0x28);
    self->field_0x0.Free();
}
