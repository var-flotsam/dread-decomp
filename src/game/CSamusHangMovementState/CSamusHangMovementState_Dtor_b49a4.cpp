// CSamusHangMovementState unit (tools/tu_map.py): the destructor: both vtables (+0, +0x188),
// fn_71009a1f40, the counted pointer at +0x1b8 released, then the movement-state base destructor
// (fn_71009b0800). The same body is inlined in fn_71009dfa80 (src/game_hi). Names and layouts are
// placeholders.
#include "game/CSamusHangMovementState/CParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_71009a1f40(void* p);
void fn_71009b0800(void*);

struct UnkSamusHangStateDtor_b49a4 {
    void* vtable;
    u8 pad_0x8[0x180];
    void* vtable2;
    u8 pad_0x190[0x28];
    UnkRefObject* field_0x1b8;
};

void fn_71009a2c58(UnkSamusHangStateDtor_b49a4* self) {
    self->vtable = g_unk_7101a87000 + 2;
    self->vtable2 = g_unk_7101a87000 + 48;
    fn_71009a1f40(self);
    if (UnkRefObject* p = self->field_0x1b8) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    fn_71009b0800(self);
}
