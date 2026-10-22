// Copies of fn_710046f678 (src/game_s2/fn_710046f678.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_71006ef540.
// Node routine that calls fn_71003ace0c and initialises or clears fields.
#include <new>

#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    u8 pad_0x0[0x15a0];
    u64 field_0x15a0;
    u64 field_0x15a8;
};

void fn_71003ace0c(UnkNode*);

void fn_71003695fc(UnkNode*);
void fn_71006ef540(UnkNode* self) {
    fn_71003695fc(self);
    self->field_0x15a8 = 0;
    self->field_0x15a0 = 0;
}

