// CKraidAcidBlobsAttack unit (tools/tu_map.py, the name from reflection): restarts the attack. After the
// base restart (fn_71005f2018) it clears the flag at +0x38 and the counters at +0x40 and +0x48, then picks
// the next pattern at +0x44: 0 when the owner (+0x30) has one pattern (+0x118), otherwise a random one
// different from the current. The single-pattern case is laid out first (hence the expect). Names and
// layouts are placeholders.
#include "types.h"

extern "C" int rand();
void fn_71005f2018(char*);

struct UnkKraidAcidOwner_b54 {
    u8 pad_0x0[0x118];
    u32 pattern_count;
};

struct UnkKraidAcidBlobsAttack_b54 {
    u8 pad_0x0[0x30];
    UnkKraidAcidOwner_b54* owner;
    bool flag;  // 0x38
    u8 pad_0x39[7];
    u32 field_0x40;
    u32 pattern;  // 0x44
    u32 field_0x48;
};

void fn_71005f12b0(UnkKraidAcidBlobsAttack_b54* self) {
    fn_71005f2018(reinterpret_cast<char*>(self));
    self->flag = false;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    u32 others = self->owner->pattern_count - 1;
    if (__builtin_expect(others != 0, 0)) {
        u32 next = (u32)(rand() & 0x7fff) % others;
        if (next >= self->pattern)
            ++next;
        self->pattern = next;
    } else {
        self->pattern = 0;
    }
}
