#pragma once

// The Chozo commander's ki grab attack (reflected as CChozoCommanderKiGrabAttack; unit 652 of tools/tu_map.py),
// as far as the hand-written sources know it: one layout, grown by each source that reads a member it does not
// have yet, and the hash of the request a kind of grab sends. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "types.h"

// The hashes of the kinds without a name of their own: one table for the owner's character class, one for
// the player's.
extern const u64 g_unk_71016494b8[] __attribute__((visibility("hidden")));
extern const u64 g_unk_7101649478[] __attribute__((visibility("hidden")));

// The member at +0x98, when there is one: the limit the next step is given.
struct UnkKiGrabLimit {
    u8 pad_0x0[0x10];
    u32 limit;  // 0x10
};

struct UnkKiGrabAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x98 - 0x10];
    UnkKiGrabLimit* limit;  // 0x98
    u8 pad_0xa0[0x13c - 0xa0];
    bool field_0x13c;
    bool field_0x13d;  // the Samus sequences take their "aura" names (else "counter")
    u8 pad_0x13e[2];
    f32 field_0x140;
    f32 field_0x144;
    f32 field_0x148;
    f32 field_0x14c;
};
static_assert(__builtin_offsetof(UnkKiGrabAttack, field_0x140) == 0x140, "UnkKiGrabAttack placeholder layout");

// The value a request carries (twelve bytes, copied as a whole: an 8-byte and a 4-byte move).
struct UnkKiGrabValue {
    u32 field_0x0;
    u32 field_0x4;
    f32 value;
};

// The hash for a kind: kinds 3 and 4 hash a Samus sequence name, the rest come from `table`.
static inline u64 KiGrabHash(UnkKiGrabAttack* self, s32 kind, const u64* table) {
    switch (kind) {
    case 3:
        if (self->field_0x13d)
            return StringCrc64("kigrabcommandersamusseq1prep_aura", 33);
        return StringCrc64("kigrabcommandersamusseq1prep_counter", 36);
    case 4:
        if (self->field_0x13d)
            return StringCrc64("kigrabcommandersamusseq1_aura", 29);
        return StringCrc64("kigrabcommandersamusseq1_counter", 32);
    default:
        return table[kind];
    }
}
