// CTentacle (reflection: size 0x40): its copy constructor (fn_710070baec) and move
// constructor (fn_710070bc3c), both the implicit copy.
#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"

#include <new>

struct CTentacle {
    virtual ~CTentacle();
    base::global::CStrId field_0x8;
    base::UnkStrIdVec field_0x10;
    u32 field_0x38;
    bool field_0x3c;
};

void fn_710070baec(CTentacle* self, const CTentacle* other) {
    new (self) CTentacle(*other);
}

void fn_710070bc3c(CTentacle* self, const CTentacle* other) {
    new (self) CTentacle(*other);
}
