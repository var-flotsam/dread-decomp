// base::snd::CSoundSystemATK (unit 99): setters that cache a flag in the sound system and queue a
// command for the sound thread through fn_71000faac4 (fn_71000fbc38, fn_71000fbbe0, fn_71000fbc7c),
// and the queue of the second buffer set (fn_71000fbaf8). Names and offsets are placeholders.
#include <string.h>

#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/snd/UnkSoundSystemATK.h"

using base::snd::UnkSoundSystemATK;

// The second command queue's members (offsets past the first queue's members).
struct UnkSoundQueue2View {
    u8 pad_0x0[0x514];
    s32 field_0x514;  // the buffer being filled
    u8 pad_0x518[4];
    s32 field_0x51c;  // bytes used in it
    u8 pad_0x520[0x200000];
    u8 field_0x200520[2][0x100000];
    u8 pad_0x400520[0x28];
    nn::os::MutexType field_0x400548;
};
static_assert(__builtin_offsetof(UnkSoundQueue2View, field_0x400548) == 0x400548, "layout");

// The payload size of each command of the second queue.
extern const s32 g_unk_7101647aa8[] __attribute__((visibility("hidden")));

// fn_71000faac4 (include/base/snd/UnkSoundSystemATK.h) under a local name whose payload parameter is
// marked noescape: in the original unit the definition is visible, so its payload is known not to be
// captured, and fn_71000fbbe0 can still tail-call after the inlined setter.
void QueueSoundCommand(UnkSoundSystemATK* self, s32 command, __attribute__((noescape)) const void* payload) asm(
    "_Z13fn_71000faac4PN4base3snd17UnkSoundSystemATKEiPKv");

void fn_71000fa05c(UnkSoundSystemATK* self, f32 value);
void fn_71000fbce0(UnkSoundSystemATK* self, bool on, const s32* mask);

// A command's payload (one word, 8-byte aligned).
struct alignas(8) UnkWordPayload {
    s32 value;
};

static inline u8& Flag(UnkSoundSystemATK* self, u32 offset) {
    return reinterpret_cast<u8*>(self)[offset];
}

void fn_71000fbc38(UnkSoundSystemATK* self, bool on) {
    if (Flag(self, 0x344) != on) {
        Flag(self, 0x344) = on;
        UnkWordPayload value = {on};
        QueueSoundCommand(self, 0x26, &value);
    }
}

void fn_71000fbbe0(UnkSoundSystemATK* self, f32 value) {
    if (Flag(self, 0x344))
        fn_71000fbc38(self, false);
    fn_71000fa05c(self, value);
}

void fn_71000fbc7c(UnkSoundSystemATK* self, bool on) {
    if (Flag(self, 0x342) != on) {
        bool off = !on;
        alignas(8) s32 mask = 0x49;
        fn_71000fbce0(self, off, &mask);
        Flag(self, 0x342) = on;
        UnkWordPayload value = {on};
        fn_71000faac4(self, 0x25, &value);
    }
}

void fn_71000fbaf8(UnkSoundSystemATK* sys, s32 command, const void* payload) {
    UnkSoundQueue2View* self = reinterpret_cast<UnkSoundQueue2View*>(sys);
    nn::os::GetCurrentThread();
    s32 size = g_unk_7101647aa8[command];
    nn::os::MutexType* mutex = &self->field_0x400548;
    fn_710006691c(mutex);
    while (size + 4 + self->field_0x51c >= 0x100000) {
        fn_7100066920(mutex);
        nn::os::SleepThread(nn::TimeSpan::FromNanoSeconds(1000000));
        fn_710006691c(mutex);
    }
    u8* buffer = self->field_0x200520[self->field_0x514];
    *reinterpret_cast<s32*>(buffer + self->field_0x51c) = command;
    self->field_0x51c += 4;
    memcpy(buffer + self->field_0x51c, payload, size);
    self->field_0x51c += size;
    fn_7100066920(mutex);
}

// Sets the value at index `index` of the float array at +0x1f4, marking the array dirty (+0x347).
void fn_71000fbed8(UnkSoundSystemATK* self, s32 index, f32 value) {
    f32* slot = reinterpret_cast<f32*>(self) + index + 0x1f4 / 4;
    if (*slot != value) {
        asm("" : "+r"(slot));  // the original forms the address again for the store
        *slot = value;
        Flag(self, 0x347) = true;
    }
}

