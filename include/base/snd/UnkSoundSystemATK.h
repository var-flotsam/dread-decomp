#pragma once

// The sound system object of unit 99 (tools/tu_map.py: 0x71000f43c0-0x71001043e4, whose reflected
// class is base::snd::CSoundSystemATK::CTunableSoundSystemATK). Its setters (fn_71000fbc38,
// fn_71000fbc7c, fn_71000fbbe0 ...) cache a value in a member and queue a command for the sound
// thread through fn_71000faac4, which appends records to one of its command buffers under a mutex.
// Only the members touched so far are spelled out; names and layout are placeholders, and the
// object's size is unknown.

#include <nn/os.h>

#include "types.h"

namespace base::snd {

struct UnkSoundSystemATK {
    u8 pad_0x0[0x500];
    s32 field_0x500;  // the command buffer being filled
    u8 pad_0x504[4];
    s32 field_0x508;  // bytes used in it
    u8 pad_0x50c[0x14];
    u8 field_0x520[4][0x100000];  // the command buffers
    u32 field_0x400520;           // the next record's sequence number
    u8 pad_0x400524[4];
    nn::os::MutexType field_0x400528;  // guards the buffers and the counters
};
static_assert(__builtin_offsetof(UnkSoundSystemATK, field_0x508) == 0x508, "UnkSoundSystemATK placeholder layout");
static_assert(__builtin_offsetof(UnkSoundSystemATK, field_0x400520) == 0x400520, "UnkSoundSystemATK placeholder layout");
static_assert(__builtin_offsetof(UnkSoundSystemATK, field_0x400528) == 0x400528, "UnkSoundSystemATK placeholder layout");

}  // namespace base::snd

// Queues command `command` with its payload (the size comes from a table indexed by the command) for
// the sound thread: a record of the command, a sequence number and the payload, appended to the
// current buffer. While the buffer cannot take it, waits a millisecond at a time with the mutex
// released. Does nothing while the global flag g_unk_7108b6ab60 is set. Defined in
// src/base/snd/CSoundSystemATK.cpp.
void fn_71000faac4(base::snd::UnkSoundSystemATK* self, s32 command, const void* payload);

#include "base/global/CFilePathStrId.h"

// Returns the file-path id for the lowercased name of `id` with every character outside a-z and 0-9
// replaced by '_' (the name goes through the 0x7108b6b910 scratch buffer), caching the result in the
// map at g_unk_7101d685c8 (keyed by the id's hash); an id passed in as empty comes back as it is. While
// the global flag g_unk_7108b6ab60 is set it returns the global id g_unk_7101d470e8 instead. The
// unit this belongs to (99, the sound system's) is only the address range's. Defined in
// src/base/snd/CSoundSystemATK.cpp.
base::global::CFilePathStrId fn_71000f63b0(const base::global::CFilePathStrId& id);
