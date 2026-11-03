// (src/game/UnkUnit330/UnkUnit330Flush.cpp holds another function of the unit.)
// Unit 330 of tools/tu_map.py (0x7100224d44-0x710022554c): a playing media instance (the one
// src/base/spatial/UnkModelInstanceSet.cpp calls UnkMediaInstance, state at +0x2c) with an array of
// tracks at +0x60 (count at +0x68), a lock at +0x960 and a player at +0xad8. Names and layouts are
// placeholders.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

struct UnkMediaInstance;
bool fn_710021eec0(UnkMediaInstance* instance, s32 state);

struct UnkMediaTrack330;
void fn_710021b7d4(UnkMediaTrack330* track);
void fn_710021b860(UnkMediaTrack330* track, bool a, bool b);
void fn_710021b9a8(UnkMediaTrack330* track);

struct UnkMediaPlayer330 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(void* arg);
};

struct UnkMedia330 {
    u8 pad_0x0[0x2c];
    s32 state;  // 0x2c
    u8 pad_0x30[0x30];
    UnkMediaTrack330** tracks;  // 0x60
    u32 track_count;            // 0x68
    u8 pad_0x6c[0x8f4];
    nn::os::MutexType lock;  // 0x960
    u8 pad_0x980[0x158];
    UnkMediaPlayer330* player;  // 0xad8
};
static_assert(__builtin_offsetof(UnkMedia330, player) == 0xad8, "UnkMedia330 layout");

void fn_710021f168(UnkMedia330* self, s32 state);
void fn_7100220228(UnkMedia330* self, u32 a);
void fn_71002202d8(UnkMedia330* self, u32 a);

void fn_7100224de4(UnkMedia330* self) {
    for (UnkMediaTrack330** it = self->tracks, **end = it + self->track_count; it != end; ++it)
        fn_710021b7d4(*it);
    if (fn_710021eec0(reinterpret_cast<UnkMediaInstance*>(self), 5))
        fn_71002202d8(self, 50);
    fn_7100220228(self, 100);
}

bool fn_7100224fbc(UnkMedia330* self, void* arg) {
    fn_710006691c(&self->lock);
    if (self->state >= 6) {
        for (UnkMediaTrack330** it = self->tracks, **end = it + self->track_count; it != end; ++it) {
            fn_710021b860(*it, true, true);
            fn_710021b9a8(*it);
        }
        self->player->vf_0x40(arg);
        fn_710021f168(self, 9);
        for (UnkMediaTrack330** it = self->tracks, **end = it + self->track_count; it != end; ++it)
            fn_710021b860(*it, true, false);
    } else {
        self->player->vf_0x40(arg);
        fn_710021f168(self, 9);
    }
    fn_7100066920(&self->lock);
    return true;
}

void fn_710021b8a4(void* entry);  // as src/game/UnkUnit330/UnkUnit330Flush.cpp declares it
void fn_71002207bc(void* part);

// Stops it: each track first (fn_710021b8a4), the player (vf_0x40 with nothing), the timer at
// +0xae0, then each track again (fn_710021b7d4), under the lock.
void fn_7100224f2c(UnkMedia330* self) {
    fn_710006691c(&self->lock);
    for (UnkMediaTrack330** it = self->tracks, **end = it + self->track_count; it != end; ++it)
        fn_710021b8a4(*it);
    self->player->vf_0x40(nullptr);
    fn_71002207bc(reinterpret_cast<char*>(self) + 0xae0);
    for (UnkMediaTrack330** it = self->tracks, **end = it + self->track_count; it != end; ++it)
        fn_710021b7d4(*it);
    fn_7100066920(&self->lock);
}
