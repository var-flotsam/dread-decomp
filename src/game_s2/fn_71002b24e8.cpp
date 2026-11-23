// Node routine that calls fn_71002b2558 and releases the string id at +0x0.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode_2b24e8 {
    UnkStrIdTwoStep field_0x0;
};

void fn_71002b2558(void*);

void fn_71002b24e8(UnkNode_2b24e8* self) {
    fn_71002b2558(reinterpret_cast<char*>(self) + 0x18);
    self->field_0x0.~UnkStrIdTwoStep();
}
