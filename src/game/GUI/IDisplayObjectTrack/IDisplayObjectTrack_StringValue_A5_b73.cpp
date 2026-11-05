// GUI::IDisplayObjectTrack unit (tools/tu_map.py, unit 1671): gets a track's text value as a string
// (fn_7100b1a1d0) and hands it, with the track's key (+0x28), to `out` (fn_7100b1a340). Names and layouts are
// placeholders.
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

struct UnkTrack_A5_b73 {
    u8 pad_0x0[0x28];
    u64 key;
};
struct UnkTrackOut_A5_b73;

CRntString fn_7100b1a1d0(UnkTrack_A5_b73* self, void* source, bool flag);
void fn_7100b1a340(UnkTrackOut_A5_b73* out, const u64* key, const char* text);

void fn_7100b1a0c0(UnkTrack_A5_b73* self, void* source, bool flag, UnkTrackOut_A5_b73* out) {
    u64 key = self->key;
    CRntString text = fn_7100b1a1d0(self, source, flag);
    fn_7100b1a340(out, &key, text.data ? text.data : "");
}
