// base::animation::CAnimationResource's unit (tools/tu_map.py unit 11): finds the track with the given
// id among the resource's tracks (+0x60) and, when there is one, applies it to the matrix at the time
// scaled by the track list's rate (fn_7100012b60). Names and layouts are placeholders.
#include "base/UnkAnimTrack_b67a1.h"
#include "base/UnkMatrix44.h"
#include "game/UnkTU20.h"
#include "types.h"

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

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
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
