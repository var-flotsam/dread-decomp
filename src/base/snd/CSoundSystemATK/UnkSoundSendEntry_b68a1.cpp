// base::snd::CSoundSystemATK (unit 99): fn_71000ff958 checks the calling thread (fn_7100132cd0) and,
// for a given entry, queues its word at +0xc4 for the sound thread as command 0x15 when the entry's
// flag at +0x120 is set, else as command 8. The two payloads keep separate stack slots (the goto skips
// the first one's declaration, so it has no lifetime markers). Names and offsets are placeholders.
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "types.h"

using base::snd::UnkSoundSystemATK;

namespace {

// A command's payload (one word, 8-byte aligned).
struct alignas(8) UnkWordPayload_b68a1 {
    s32 value;
};

}  // namespace

struct UnkSoundEntry_b68a1 {
    u8 pad_0x0[0xc4];
    s32 field_0xc4;
    u8 pad_0xc8[0x58];
    bool field_0x120;
};

void fn_71000ff958(UnkSoundSystemATK* self, UnkSoundEntry_b68a1* entry) {
    fn_7100132cd0();
    if (!entry)
        return;
    if (!entry->field_0x120)
        goto hidden;
    UnkWordPayload_b68a1 shown;
    shown.value = entry->field_0xc4;
    fn_71000faac4(self, 0x15, &shown);
    return;
hidden:
    UnkWordPayload_b68a1 other;
    other.value = entry->field_0xc4;
    fn_71000faac4(self, 8, &other);
}
