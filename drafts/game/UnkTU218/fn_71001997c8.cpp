// UnkTU218: copies one of two per-slot vectors of a table entry and hands it on to a callee.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

struct UnkTU218Entry {
    u8 pad_0x0[0x14c];
    base::math::UnkVector4D set1[4];
    u8 pad_0x18c[0xb4];
    base::math::UnkVector4D set0[4];
};
struct UnkTU218Table {
    u8 pad_0x0[0x48];
    UnkTU218Entry** entries;
};
void fn_710007b518(base::math::CVector4D* self, const base::math::CVector4D* other);

u32 fn_71001997c8(UnkTU218Table* self, u32 index, u32 slot, base::math::CVector4D* out, u32 mode) {
    UnkTU218Entry* entry = self->entries[index];
    if (mode == 0) {
        base::math::UnkVector4D v = entry->set0[slot];
        fn_710007b518(out, &v);
    } else if (mode == 1) {
        base::math::UnkVector4D v = entry->set1[slot];
        fn_710007b518(out, &v);
    }
    return 1;
}
