// CSkeletalAnimation's unit (tools/tu_map.py unit 21): for each 0x30-byte track of the animation's
// track list (*(+8))->+0x60, looks the track's bone id up among the given matrices (fn_7100015508)
// and, when there is one, applies the track to it with the weight at +0x18 (fn_7100013be4 ->
// fn_7100012b60). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

UnkMatrix44* fn_7100015508(void* matrices, u64 id);

struct UnkAnimTrack_b67a1 {
    u64 id;
    u8 pad_0x8[0x28];
};

struct UnkAnimTrackList_b67a1 {
    u8 pad_0x0[0x10];
    s32 count;  // 0x10
    u8 pad_0x14[0x4];
    UnkAnimTrack_b67a1 tracks[1];  // 0x18
};

struct UnkAnimResource_b67a1 {
    u8 pad_0x0[0x60];
    UnkAnimTrackList_b67a1* tracks;  // 0x60
};

struct UnkAnimInstance_b67a1 {
    u8 pad_0x0[0x8];
    UnkAnimResource_b67a1* resource;  // 0x8
    u8 pad_0x10[0x8];
    f32 weight;  // 0x18
};

void fn_7100012b60(UnkMatrix44* m, const UnkAnimTrack_b67a1* track, f32 weight);

void fn_7100013be4(UnkAnimInstance_b67a1* self, void* matrices) {
    UnkAnimTrackList_b67a1* list = self->resource->tracks;
    for (s32 i = 0; i < list->count; ++i) {
        UnkAnimTrack_b67a1* track = &list->tracks[i];
        if (UnkMatrix44* m = fn_7100015508(matrices, track->id))
            fn_7100012b60(m, track, self->weight);
    }
}
