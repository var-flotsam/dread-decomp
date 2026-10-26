// Node routine that calls fn_71002b6094 and releases the string id at +0x0.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkObject_0x100.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    UnkStrIdTwoStep field_0x0;
    UnkObject_0x100 field_0x8;
};

void fn_71002b5698(UnkNode* self) {
    fn_71002b6094(&self->field_0x8);
    self->field_0x0.~UnkStrIdTwoStep();
}
