// Unit 324: waits for every track of a media object to report ready (vf_0x30), sleeping between
// polls, up to 125 rounds in mode 4 or limit / 10 rounds otherwise. Names are placeholders.
#include "game/UnkTU58.h"
#include "types.h"

struct UnkMedia330;

struct UnkMediaMode_b0217 {
    u8 pad_0x0[8];
    s32 kind;  // 0x8
};

struct UnkMediaTrackV_b0217 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool vf_0x30();
};

struct UnkMediaLayout2_b0217 {
    u8 pad_0x0[0x58];
    UnkMediaMode_b0217* mode;  // 0x58
    UnkMediaTrackV_b0217** tracks;  // 0x60
    u32 track_count;                // 0x68
};

void fn_7100220228(UnkMedia330* media, s64 limit) {
    UnkMediaLayout2_b0217* self = reinterpret_cast<UnkMediaLayout2_b0217*>(media);
    s64 rounds;
    if (self->mode && self->mode->kind == 4)
        rounds = 125;
    else if (limit >= 10) {
        // keeps the signed divide: the range check above must not narrow it to unsigned
        asm("" : "+r"(limit));
        rounds = limit / 10;
    } else
        return;
    s64 done = 0;
    while (self->track_count) {
        done++;
        bool all = true;
        UnkMediaTrackV_b0217** it = self->tracks;
        for (UnkMediaTrackV_b0217** end = it + self->track_count; it != end; ++it)
            all &= (*it)->vf_0x30();
        if (all)
            break;
        fn_71000810b0(1);
        if (done == rounds)
            break;
    }
}
