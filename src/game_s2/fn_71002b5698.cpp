// Node routine that calls fn_71002b6094 and releases the string id at +0x0.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkObject_0x100.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode_71002b5698 {
    UnkStrIdTwoStep field_0x0;
    UnkObject_0x100 field_0x8;
};

void fn_71002b5698(UnkNode_71002b5698* self) {
    fn_71002b6094(&self->field_0x8);
    self->field_0x0.~UnkStrIdTwoStep();
}
