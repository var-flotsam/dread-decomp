// CShootDCBones (reflection: size 0x38): its copy constructor (fn_71006197f8) and move
// constructor (fn_7100619938), both the implicit copy.
#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"

#include <new>

struct CShootDCBones {
    virtual ~CShootDCBones();
    base::global::CStrId field_0x8;
    base::UnkStrIdVec field_0x10;
};

void fn_71006197f8(CShootDCBones* self, const CShootDCBones* other) {
    new (self) CShootDCBones(*other);
}

void fn_7100619938(CShootDCBones* self, const CShootDCBones* other) {
    new (self) CShootDCBones(*other);
}
