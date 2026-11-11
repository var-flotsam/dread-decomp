// Unit 2685 (no reliable name): a named set of reflected values with its byte stream. Two string ids
// (a copy of the caller's, and one made from a string), the array of reflected values at +0x10 (flags 5), the
// hash stream at +0x38 (kind 0x16) and two words. Names and layouts are placeholders.
#include <new>

#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU2685.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkValueElem_b0225 : base::reflection::UnkReflValue {
    ~UnkValueElem_b0225() { fn_71000a1c60(this); }
};
static_assert(sizeof(UnkValueElem_b0225) == 0x18, "element layout");

struct UnkValueSet_b0225 {
    union {
        CStrId name;  // 0x0
    };
    union {
        CStrId key;  // 0x8
    };
    base::UnkVector<UnkValueElem_b0225> values;  // 0x10
    UnkHashStream_0x38 stream;                   // 0x38
    void* arg;                                   // 0x70
    bool flag;                                   // 0x78
};

void fn_71010c7284(UnkValueSet_b0225* self, const CStrId* name, const CRntString* key, bool flag, void* arg) {
    new (&self->name) CStrId(*name);
    fn_7100000850(&self->key, *key, true);
    new (&self->values) base::UnkVector<UnkValueElem_b0225>(5);
    fn_710007be8c(&self->stream, 0x16, nullptr);
    self->arg = arg;
    self->flag = flag;
}

void fn_71010c7328(UnkValueSet_b0225* self) {
    fn_710007c0e4(&self->stream);
    self->values.Free();
    self->key.Reset();
    self->name.Reset();
}
