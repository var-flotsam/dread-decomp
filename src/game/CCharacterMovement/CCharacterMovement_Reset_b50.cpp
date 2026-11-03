// CCharacterMovement unit (tools/tu_map.py, the name from reflection): the base reset
// (fn_71010d2f84), then drops the counted references in the list at +0x3f8 (count at +0x400) and
// clears the list, the flag at +0x3aa and +0x3c0. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkMovementContact {
    UnkRefObject* ref;
    u64 field_0x8;
};

struct UnkCharacterMovementReset {
    u8 unk_0x0[0x3aa];
    bool field_0x3aa;
    u8 unk_0x3ab[0x15];
    u64 field_0x3c0;
    u8 unk_0x3c8[0x30];
    UnkMovementContact* contacts;  // 0x3f8
    u32 count;                     // 0x400
};

void fn_71010d2f84(UnkCharacterMovementReset* self, bool flag);

void fn_71009786a8(UnkCharacterMovementReset* self, bool flag) {
    fn_71010d2f84(self, flag);
    for (u64 i = 0; i < self->count; i++) {
        if (UnkRefObject* p = self->contacts[i].ref) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
    self->count = 0;
    self->field_0x3aa = false;
    self->field_0x3c0 = 0;
}
