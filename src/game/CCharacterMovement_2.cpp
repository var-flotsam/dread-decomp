// In the CCharacterMovement unit: clears the flag at +0x396 and hands the owner at +0x30 and the
// block at +0x280 to the collision manager (created on first use, as
// src/game/CCollisionComponent/fn_7100863a8c.cpp does) through fn_7100d47d1c. Names and layouts
// are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkManager_7100863a8c;
extern UnkManager_7100863a8c* g_unk_71726a4788;
void fn_7100d46804(UnkManager_7100863a8c* self);
void fn_7100d47d1c(UnkManager_7100863a8c* self, void* owner, void* block);

struct UnkMovement_71009796dc {
    u8 pad_0x0[0x30];
    void* field_0x30;
    u8 pad_0x38[0x248];
    u8 field_0x280[0x116];
    bool field_0x396;
};

void fn_71009796dc(UnkMovement_71009796dc* self) {
    self->field_0x396 = false;
    void* block = self->field_0x280;
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    fn_7100d47d1c(m, self->field_0x30, block);
}
