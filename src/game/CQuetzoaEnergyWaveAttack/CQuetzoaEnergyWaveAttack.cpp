// Refers the weak reference at +0x14a20 to another object's weak handle, as fn_7100ed77b8
// (src/game/CShotManager/CTunableShotManager/fn_7100ed77b8.cpp): fn_7100646088.
#include "game_s2/UnkWeakHandle.h"

struct UnkWeakRef14a20 {
    u8 unk_0x0[0x14a20];
    UnkWeakHandle* field_0x14a20;
};

void fn_7100646088(UnkWeakRef14a20* self, UnkWeakTarget* other) {
    UnkWeakHandle* handle = GetWeakHandle(other);
    asm("" : "+r"(self));
    AssignRef(&self->field_0x14a20, handle);
}
