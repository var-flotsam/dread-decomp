// Refers a weak reference to another object's weak handle, as fn_7100b42be0
// (src/game_aca57c/fn_7100b021fc.cpp): fn_7100ed77b8.
#include "game_s2/UnkWeakHandle.h"

struct UnkWeakRef1268 {
    u8 unk_0x0[0x1268];
    UnkWeakHandle* field_0x1268;
};

void fn_7100ed77b8(UnkWeakRef1268* self, UnkWeakTarget* other) { AssignRef(&self->field_0x1268, GetWeakHandle(other)); }
