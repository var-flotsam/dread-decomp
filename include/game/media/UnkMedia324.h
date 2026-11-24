#pragma once

// The media instance of units 324 and 330 (tools/tu_map.py; what src/base/spatial/UnkModelInstanceSet.cpp
// calls UnkMediaInstance): a state at +0x2c with the one it left at +0x30, an array of tracks, listeners
// told of each state change under the inner lock (+0x940), the outer lock (+0x960), a ring of queued
// commands and the player at +0xad8. Names are placeholders; the offsets are the ones the functions of
// both units read.
#include <nn/os.h>

#include "types.h"

struct UnkMedia324;
struct UnkMediaTrack330;   // a track (src/game/media/UnkUnit330/UnkUnit330.cpp)
struct UnkMediaPlayer330;  // the player (src/game/media/UnkUnit330/UnkUnit330.cpp)

// A listener: a context and a callback, called with the instance on each state change.
struct UnkMediaListener324 {
    void* context;
    void (*callback)(void* context, UnkMedia324* media, u32 arg);
};

// The fixed ring of 64 words of UnkMovieUnit324_Rings.cpp.
struct UnkMovieWordRing324 {
    void* vtable;
    u32* read;
    u32* write;
    u64 count;
    u32 entries[64];

    void Push(u32 value) {
        // A store the compiler can't tell apart from the ring's own fields: the position is read again.
        typedef u32 AnyWord;
        *reinterpret_cast<AnyWord*>(write) = value;
        u32* next = write + 1;
        write = next == entries + 64 ? entries : next;
        ++count;
    }

    u32 PopFront() {
        u32 value = *read;
        u32* next = read + 1;
        read = next == entries + 64 ? entries : next;
        --count;
        return value;
    }
};

struct UnkMedia324 {
    u8 pad_0x0[0x2c];
    s32 state;           // 0x2c
    s32 previous_state;  // 0x30
    s32 seek_state;      // 0x34
    u8 pad_0x38[0x28];
    UnkMediaTrack330** tracks;  // 0x60
    u32 track_count;            // 0x68
    u8 pad_0x6c[0x6b4];
    UnkMediaListener324* listeners;  // 0x720
    u32 listener_count;              // 0x728
    u8 pad_0x72c[0x1c];
    u8 worker[0x1f8];                 // 0x748, a thread (fn_7100066828 wakes it)
    nn::os::MutexType listener_lock;  // 0x940
    nn::os::MutexType lock;           // 0x960
    nn::os::LightEventType ready;     // 0x980
    u8 pad_0x98c[0x4];
    nn::os::MutexType command_lock;  // 0x990
    UnkMovieWordRing324 commands;    // 0x9b0
    u32 loop_mode;                   // 0xad0
    bool start_pending;              // 0xad4
    UnkMediaPlayer330* player;       // 0xad8
    u8 field_0xae0[1];               // 0xae0, a timer (fn_71002207bc stops it)
};
static_assert(__builtin_offsetof(UnkMedia324, lock) == 0x960, "UnkMedia324 layout");
static_assert(__builtin_offsetof(UnkMedia324, loop_mode) == 0xad0, "UnkMedia324 layout");
static_assert(__builtin_offsetof(UnkMedia324, player) == 0xad8, "UnkMedia324 layout");
