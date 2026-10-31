// Resizes an array of 0x60-byte entries (the one fn_7100fddcb4 grows): reserves the new count,
// then default-constructs each added entry. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

struct UnkNewEntry_0x60 {
    u32 field_0x0 = 0;
    u32 field_0x4 = 1;
    f32 field_0x8 = 1.0f;
    u32 field_0xc = 0;
    f32 field_0x10 = 0.0f;
    f32 field_0x14 = 1.0f;
    u32 field_0x18 = 0;
    u32 field_0x1c = 0;
    u32 field_0x20 = 0;
    u8 field_0x24 = 0;
    u8 field_0x25 = 0;
    bool field_0x26 = true;
    base::UnkVector<u8[0x18]> field_0x28{0xf};
    u64 field_0x50 = 0;
    base::global::CStrId field_0x58{"", true};
};

void fn_7100fde4d8(base::UnkVector<UnkNewEntry_0x60>* v, u32 n) {
    fn_7100fddcb4(reinterpret_cast<base::UnkVector<UnkMusicEntry_0x60>*>(v), n);  // reserve
    for (u32 i = v->count; i < n; ++i)
        new (&v->data[i]) UnkNewEntry_0x60;
    v->count = n;
}
