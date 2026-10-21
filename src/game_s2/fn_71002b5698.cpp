// Node routine that calls fn_71002b6094 and releases the string id at +0x0.
#include <new>

#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    UnkStrIdTwoStep field_0x0;
};

void fn_71002b6094(void*);

void fn_71002b5698(UnkNode* self) {
    fn_71002b6094(reinterpret_cast<char*>(self) + 0x8);
    self->field_0x0.~UnkStrIdTwoStep();
}
