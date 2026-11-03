// Unit 99 (base::snd::CSoundSystemATK), beside CSoundSystemATK.cpp's voice commands: the number of
// voices in the first list, read under the voice mutex (0 while the global flag g_unk_7108b6ab60 is
// set). Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

// Declared as in src/base/snd/CSoundSystemATK.cpp.
extern u8 g_unk_7108b6ab60 __attribute__((visibility("hidden")));
bool fn_7100132cd0();

struct UnkSoundSystemVoiceCount {
    u8 pad_0x0[0x3f8];
    u32 count;  // 0x3f8, the first voice list's
    u8 pad_0x3fc[0x400568 - 0x3fc];
    nn::os::MutexType mutex;  // 0x400568
};
static_assert(__builtin_offsetof(UnkSoundSystemVoiceCount, mutex) == 0x400568, "placeholder layout");

u32 fn_71000fd2a4(UnkSoundSystemVoiceCount* self) {
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return 0;
    fn_7100132cd0();
    nn::os::MutexType* mutex = &self->mutex;
    fn_710006691c(mutex);
    u32 count = self->count;
    fn_7100066920(mutex);
    return count;
}
