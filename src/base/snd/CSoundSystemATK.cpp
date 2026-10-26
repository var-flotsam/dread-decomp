// The sound system of unit 99 (base::snd::CSoundSystemATK): the command queue its setters feed.
#include <string.h>

#include "base/UnkDefaultAllocator.h"
#include "base/snd/UnkSoundSystemATK.h"

using base::snd::UnkSoundSystemATK;

// A flag that turns the queue off (its meaning is unverified); a direct (not GOT) access.
extern u8 g_unk_7108b6ab60 __attribute__((visibility("hidden")));
// The payload size of each command.
extern const s32 g_unk_7101647a0c[] __attribute__((visibility("hidden")));

// Probably whether the caller is on the sound thread (it compares a stored thread id with the
// current one); the result is not used here.
bool fn_7100132cd0();

void fn_71000faac4(UnkSoundSystemATK* self, s32 command, const void* payload) {
    // The hint only steers the block layout: the original keeps the return next to the test.
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    s32 size = g_unk_7101647a0c[command];
    nn::os::MutexType* mutex = &self->field_0x400528;
    fn_710006691c(mutex);
    while (size + 4 + self->field_0x508 >= 0x100000) {
        fn_7100066920(mutex);
        nn::os::SleepThread(nn::TimeSpan::FromNanoSeconds(1000000));
        fn_710006691c(mutex);
    }
    u8* buffer = self->field_0x520[self->field_0x500];
    *reinterpret_cast<s32*>(buffer + self->field_0x508) = command;
    self->field_0x508 += 4;
    s32 sequence = self->field_0x400520++;
    *reinterpret_cast<s32*>(buffer + self->field_0x508) = sequence;
    self->field_0x508 += 4;
    memcpy(buffer + self->field_0x508, payload, size);
    self->field_0x508 += size;
    fn_7100066920(mutex);
}
