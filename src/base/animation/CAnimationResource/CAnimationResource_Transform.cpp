// base::animation::CAnimationResource's unit (unit 11 of tools/tu_map.py): fills a 4x4 matrix
// for a sample: through the track set at +0x68 (fn_7100012d6c) with the two times scaled by the
// rate at +0xc of the header at +0x60, or the identity when there is no track set. Names are
// placeholders.
#include "types.h"

struct UnkMatrix44_7100004f50 {
    f32 m[16];
};

struct UnkAnimHeader_7100004f50 {
    u8 unk_0x0[0xc];
    f32 rate;
};

struct UnkTrackSet_7100004f50;

void fn_7100012d6c(UnkTrackSet_7100004f50* tracks, bool a, bool b, UnkMatrix44_7100004f50* out, f32 t0, f32 t1,
                   f32 rate);

struct UnkAnimResource_7100004f50 {
    u8 unk_0x0[0x60];
    UnkAnimHeader_7100004f50* header;
    UnkTrackSet_7100004f50* tracks;
};

void fn_7100004f50(UnkAnimResource_7100004f50* self, UnkMatrix44_7100004f50* out, bool a, bool b, f32 t0, f32 t1) {
    if (self->tracks) {
        f32 rate = self->header->rate;
        fn_7100012d6c(self->tracks, a, b, out, rate * t0, rate * t1, rate);
        return;
    }
    __builtin_memset(out, 0, sizeof(*out));
    out->m[15] = 1.0f;
    out->m[10] = 1.0f;
    out->m[5] = 1.0f;
    out->m[0] = 1.0f;
}
