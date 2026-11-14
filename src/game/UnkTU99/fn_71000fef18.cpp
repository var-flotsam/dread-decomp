// UnkTU99 (sound system): counts the entries of a locked pointer list whose channel number is set in a bit mask.
// Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU99.h"
#include "types.h"

struct UnkMaskedEntry {
    u8 pad_0x0[0xb0];
    u32 channel;
};

struct UnkMaskedOwner {
    u8 pad_0x0[0x418];
    UnkMaskedEntry** entries;
    u32 count;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;
};

// NON_MATCHING(minor): the epilogue is duplicated on the early-out path instead of branching to a shared one
u32 fn_71000fef18(void* member, u32 mask) {
    UnkMaskedOwner* self = static_cast<UnkMaskedOwner*>(member);
    u32 total = 0;
    if (__builtin_expect(!(g_unk_7108b6ab60 & 1), 0)) {
        fn_7100132cd0();
        fn_710006691c(&self->mutex);
        UnkMaskedEntry** end = self->entries + self->count;
        for (UnkMaskedEntry** it = self->entries; it != end; ++it) {
            if ((1u << (*it)->channel) & mask) {
                total++;
            }
        }
        fn_7100066920(&self->mutex);
    }
    return total;
}
