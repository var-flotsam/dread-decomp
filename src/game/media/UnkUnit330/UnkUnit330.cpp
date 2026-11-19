// (src/game/UnkUnit330/UnkUnit330Flush.cpp holds another function of the unit.)
// Unit 330 of tools/tu_map.py (0x7100224d44-0x710022554c): a playing media instance (the one
// src/base/spatial/UnkModelInstanceSet.cpp calls UnkMediaInstance, state at +0x2c) with an array of
// tracks at +0x60 (count at +0x68), a lock at +0x960 and a player at +0xad8. Names and layouts are
// placeholders.
#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU324.h"
#include "game/UnkTU325.h"
#include "game/media/CMediaStream.h"
#include "game/media/UnkMedia324.h"
#include "types.h"

struct UnkMediaInstance;

void fn_710021b7d4(UnkMediaTrack330* track);
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

void fn_7100220228(UnkMedia324* self, s64);
void fn_71002202d8(UnkMedia324* self, u32 a);

void fn_7100224de4(UnkMedia324* self) {
    for (UnkMediaTrack330** it = self->tracks, **end = it + self->track_count; it != end; ++it)
        fn_710021b7d4(*it);
    if (fn_710021eec0(reinterpret_cast<UnkMediaInstance*>(self), 5))
        fn_71002202d8(self, 50);
    fn_7100220228(self, 100);
}

bool fn_7100224fbc(UnkMedia324* self, void* arg) {
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

// Stops it: each track first (fn_710021b8a4), the player (vf_0x40 with nothing), the timer at
// +0xae0, then each track again (fn_710021b7d4), under the lock.
void fn_7100224f2c(UnkMedia324* self) {
    fn_710006691c(&self->lock);
    for (UnkMediaTrack330** it = self->tracks, **end = it + self->track_count; it != end; ++it)
        fn_710021b8a4(*it);
    self->player->vf_0x40(nullptr);
    fn_71002207bc(reinterpret_cast<char*>(self) + 0xae0);
    for (UnkMediaTrack330** it = self->tracks, **end = it + self->track_count; it != end; ++it)
        fn_710021b7d4(*it);
    fn_7100066920(&self->lock);
}
