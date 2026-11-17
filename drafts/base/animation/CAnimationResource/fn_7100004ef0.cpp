// base::animation::CAnimationResource's unit (tools/tu_map.py unit 11): finds the track with the given
// id among the resource's tracks (+0x60) and, when there is one, applies it to the matrix at the time
// scaled by the track list's rate (fn_7100012b60). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

// As src/base/animation/CSkeletalAnimation/CSkeletalAnimation_ApplyTracks_b67a1.cpp defines it.
struct UnkAnimTrack_b67a1 {
    u64 id;
    u8 pad_0x8[0x28];
};

void fn_7100012b60(UnkMatrix44* out, const UnkAnimTrack_b67a1* track, f32 t);

struct UnkAnimTracks_l004ef0 {
    u8 pad_0x0[0xc];
    f32 rate;   // 0xc
    s32 count;  // 0x10
    u8 pad_0x14[0x4];
    UnkAnimTrack_b67a1 tracks[1];  // 0x18
};

struct UnkAnimResource_l004ef0 {
    u8 pad_0x0[0x60];
    UnkAnimTracks_l004ef0* tracks;  // 0x60
};

bool fn_7100004ef0(UnkAnimResource_l004ef0* self, u64 id, UnkMatrix44* out, f32 t) {
    UnkAnimTracks_l004ef0* list = self->tracks;
    for (s32 i = 0; i < list->count;) {
        const UnkAnimTrack_b67a1* track = &list->tracks[i++];
        if (track->id == id) {
            fn_7100012b60(out, track, list->rate * t);
            return true;
        }
    }
    return false;
}
